#!/usr/bin/env python3
"""
Quantized .pth robustness verifier with attack + POP/SOS certificate.

Pipeline:
1) Load a real PyTorch quantized model (.pth / TorchScript).
2) Run adversarial search (PGD or SPSA) in full input space.
3) If no attack found, verify robustness on a selected coordinate subspace:
   - Fit polynomial surrogate of logit margin on that subspace.
   - Solve POP lower bound with SOS relaxation (SDP).
   - Add a deterministic residual correction using a Lipschitz bound.

The SOS certificate is deterministic for the chosen subspace if
--margin-lipschitz-inf is provided.

Example A (TorchScript quantized model):
  python3 quantized_pth_sos_verify.py \
    --model-path quant_resnet18_scripted.pth \
    --torchscript \
    --input-path sample_input.npy \
    --eps 0.01 --true-label 281 --attack auto \
    --subspace-dim 2 --poly-degree 4 --grid-points 9

Example B (state_dict + model factory):
  python3 quantized_pth_sos_verify.py \
    --model-path quant_resnet18_state_dict.pth \
    --model-factory torchvision.models.quantization.resnet18 \
    --model-kwargs-json '{"weights": null, "quantize": true}' \
    --input-path sample_input.npy \
    --eps 0.01 --true-label 281 --attack auto \
    --subspace-dim 2 --poly-degree 4 --grid-points 9
"""

from __future__ import annotations

import argparse
import importlib
import itertools
import json
import math
from dataclasses import dataclass, asdict
from pathlib import Path
from typing import Dict, List, Optional, Sequence, Tuple

import cvxpy as cp
import numpy as np
import torch
import torch.nn as nn


Exponent = Tuple[int, ...]
Poly = Dict[Exponent, object]


@dataclass
class VerificationResult:
    attack_method: str
    attack_found: bool
    attack_margin: float
    attack_label: int
    base_label: int
    target_label: int
    sos_status: str
    sos_polynomial_lb: Optional[float]
    residual_lb: Optional[float]
    certified_lb_subspace: Optional[float]
    rigorous_subspace_certificate: bool
    robust_in_verified_subspace: bool
    note: str


def parse_args() -> argparse.Namespace:
    p = argparse.ArgumentParser(
        description="Load quantized .pth model and verify robustness using POP/SOS."
    )
    p.add_argument("--model-path", type=str, required=True)
    p.add_argument("--input-path", type=str, required=True, help=".npy / .pt / .pth tensor")
    p.add_argument("--eps", type=float, required=True, help="L_inf perturbation radius")
    p.add_argument("--true-label", type=int, default=None)
    p.add_argument("--target-label", type=int, default=None)
    p.add_argument("--num-classes", type=int, default=None)
    p.add_argument("--device", type=str, default="cpu")
    p.add_argument("--clip-min", type=float, default=0.0)
    p.add_argument("--clip-max", type=float, default=1.0)

    p.add_argument("--torchscript", action="store_true", help="Load model via torch.jit.load")
    p.add_argument(
        "--model-factory",
        type=str,
        default=None,
        help="Factory path for state_dict loads, e.g. torchvision.models.resnet101",
    )
    p.add_argument(
        "--model-kwargs-json",
        type=str,
        default="{}",
        help='JSON kwargs for --model-factory, e.g. \'{"num_classes":1000}\'',
    )
    p.add_argument("--state-dict-key", type=str, default=None)
    p.add_argument("--strict-load", action="store_true")
    p.add_argument(
        "--quantized-engine",
        type=str,
        default=None,
        choices=["fbgemm", "qnnpack", "onednn", "x86"],
    )

    p.add_argument("--attack", type=str, default="auto", choices=["auto", "pgd", "spsa", "none"])
    p.add_argument("--attack-steps", type=int, default=40)
    p.add_argument("--attack-step-size", type=float, default=0.01)
    p.add_argument("--spsa-samples", type=int, default=32)
    p.add_argument("--spsa-delta", type=float, default=0.01)

    p.add_argument("--subspace-dim", type=int, default=2)
    p.add_argument("--poly-degree", type=int, default=4)
    p.add_argument("--grid-points", type=int, default=9)
    p.add_argument("--sos-multiplier-degree", type=int, default=2)
    p.add_argument(
        "--margin-lipschitz-inf",
        type=float,
        default=None,
        help=(
            "Rigorous Lipschitz bound (L_inf) of margin m(x)=logit_y-logit_t over input space. "
            "Needed for deterministic residual correction."
        ),
    )
    p.add_argument("--output-json", type=str, default=None)
    return p.parse_args()


def parse_factory_kwargs(text: str) -> dict:
    try:
        obj = json.loads(text)
    except json.JSONDecodeError as exc:
        raise ValueError(f"Invalid --model-kwargs-json: {exc}") from exc
    if not isinstance(obj, dict):
        raise ValueError("--model-kwargs-json must parse to a JSON object")
    return obj


def build_from_factory(path: str, kwargs: dict) -> nn.Module:
    if "." not in path:
        raise ValueError("--model-factory must be like module.symbol")
    module_name, symbol = path.rsplit(".", 1)
    module = importlib.import_module(module_name)
    factory = getattr(module, symbol)
    model = factory(**kwargs)
    if not isinstance(model, nn.Module):
        raise TypeError(f"{path} did not return torch.nn.Module")
    return model


def extract_state_dict(obj: object, state_dict_key: str | None) -> Dict[str, torch.Tensor]:
    if isinstance(obj, dict):
        if state_dict_key is not None:
            sd = obj[state_dict_key]
            if not isinstance(sd, dict):
                raise TypeError(f"Object at key {state_dict_key!r} is not a state_dict")
            return sd
        for key in ("state_dict", "model_state_dict", "net", "model"):
            if key in obj and isinstance(obj[key], dict):
                return obj[key]
        if all(isinstance(v, torch.Tensor) for v in obj.values()):
            return obj
    raise TypeError("Cannot extract state_dict from loaded object")


def load_model(args: argparse.Namespace) -> nn.Module:
    if args.quantized_engine is not None:
        torch.backends.quantized.engine = args.quantized_engine

    model_path = Path(args.model_path)
    map_location = torch.device(args.device)

    if args.torchscript:
        model = torch.jit.load(str(model_path), map_location=map_location)
        model.eval()
        return model

    loaded = torch.load(str(model_path), map_location=map_location)
    if isinstance(loaded, nn.Module):
        loaded.eval()
        return loaded

    if args.model_factory is None:
        raise ValueError(
            "Loaded object is not nn.Module. Provide --model-factory to rebuild architecture "
            "and load state_dict."
        )
    kwargs = parse_factory_kwargs(args.model_kwargs_json)
    model = build_from_factory(args.model_factory, kwargs)
    state_dict = extract_state_dict(loaded, args.state_dict_key)
    missing, unexpected = model.load_state_dict(state_dict, strict=args.strict_load)
    if missing or unexpected:
        print(f"[warn] load_state_dict missing={missing}, unexpected={unexpected}")
    model.eval()
    return model


def load_input_tensor(path: str, device: str) -> torch.Tensor:
    p = Path(path)
    suffix = p.suffix.lower()
    if suffix == ".npy":
        arr = np.load(str(p))
        x = torch.from_numpy(arr).float()
    else:
        obj = torch.load(str(p), map_location="cpu")
        if isinstance(obj, torch.Tensor):
            x = obj.float()
        elif isinstance(obj, dict):
            for key in ("input", "x", "data", "image"):
                if key in obj and isinstance(obj[key], torch.Tensor):
                    x = obj[key].float()
                    break
            else:
                raise TypeError("Tensor file dict must contain one of keys: input/x/data/image")
        else:
            raise TypeError("Unsupported input object format")
    if x.dim() == 3:
        x = x.unsqueeze(0)
    if x.dim() != 4:
        raise ValueError(f"Expected [N,C,H,W] or [C,H,W], got shape {tuple(x.shape)}")
    if x.shape[0] != 1:
        raise ValueError("Only batch size 1 is supported in this script")
    return x.to(device)


def model_logits(model: nn.Module, x: torch.Tensor) -> torch.Tensor:
    out = model(x)
    if isinstance(out, (tuple, list)):
        out = out[0]
    if out.dim() == 1:
        out = out.unsqueeze(0)
    if out.dim() > 2:
        out = out.reshape(out.shape[0], -1)
    return out.float()


def pick_target(logits: torch.Tensor, true_label: int, target_label: int | None) -> int:
    if target_label is not None:
        return int(target_label)
    values, indices = torch.topk(logits[0], k=2)
    if int(indices[0].item()) == true_label:
        return int(indices[1].item())
    return int(indices[0].item())


def margin_from_logits(logits: torch.Tensor, y: int, t: int) -> torch.Tensor:
    return logits[:, y] - logits[:, t]


def margin_value(model: nn.Module, x: torch.Tensor, y: int, t: int) -> float:
    with torch.no_grad():
        return float(margin_from_logits(model_logits(model, x), y, t).item())


def pgd_attack(
    model: nn.Module,
    x0: torch.Tensor,
    y: int,
    t: int,
    eps: float,
    steps: int,
    alpha: float,
    clip_min: float,
    clip_max: float,
) -> Tuple[torch.Tensor, float]:
    x = x0.detach().clone()
    x = x + torch.empty_like(x).uniform_(-eps, eps)
    x = torch.clamp(x, clip_min, clip_max)
    x = torch.max(torch.min(x, x0 + eps), x0 - eps)

    best = x.clone()
    best_margin = float("inf")

    for _ in range(steps):
        x.requires_grad_(True)
        margin = margin_from_logits(model_logits(model, x), y, t).mean()
        if not torch.isfinite(margin):
            raise RuntimeError("Non-finite margin in PGD")
        model.zero_grad(set_to_none=True)
        margin.backward()
        grad = x.grad
        if grad is None:
            raise RuntimeError("Gradient is None in PGD")
        with torch.no_grad():
            x = x - alpha * grad.sign()
            x = torch.max(torch.min(x, x0 + eps), x0 - eps)
            x = torch.clamp(x, clip_min, clip_max)
            m = margin_value(model, x, y, t)
            if m < best_margin:
                best_margin = m
                best = x.clone()
            if m <= 0.0:
                break
    return best.detach(), best_margin


def spsa_attack(
    model: nn.Module,
    x0: torch.Tensor,
    y: int,
    t: int,
    eps: float,
    steps: int,
    alpha: float,
    samples: int,
    delta: float,
    clip_min: float,
    clip_max: float,
) -> Tuple[torch.Tensor, float]:
    x = x0.detach().clone()
    x = x + torch.empty_like(x).uniform_(-eps, eps)
    x = torch.max(torch.min(x, x0 + eps), x0 - eps)
    x = torch.clamp(x, clip_min, clip_max)

    best = x.clone()
    best_margin = margin_value(model, x, y, t)

    for _ in range(steps):
        grad = torch.zeros_like(x)
        for _ in range(samples):
            u = torch.randint_like(x, low=0, high=2, dtype=torch.int64).float() * 2.0 - 1.0
            x_plus = torch.clamp(x + delta * u, clip_min, clip_max)
            x_minus = torch.clamp(x - delta * u, clip_min, clip_max)
            m_plus = margin_value(model, x_plus, y, t)
            m_minus = margin_value(model, x_minus, y, t)
            grad += ((m_plus - m_minus) / (2.0 * delta)) * u
        grad /= float(samples)
        with torch.no_grad():
            x = x - alpha * grad.sign()
            x = torch.max(torch.min(x, x0 + eps), x0 - eps)
            x = torch.clamp(x, clip_min, clip_max)
            m = margin_value(model, x, y, t)
            if m < best_margin:
                best_margin = m
                best = x.clone()
            if m <= 0.0:
                break
    return best.detach(), best_margin


def choose_coordinates_from_gradient(
    model: nn.Module, x: torch.Tensor, y: int, t: int, k: int
) -> np.ndarray:
    x_var = x.detach().clone().requires_grad_(True)
    margin = margin_from_logits(model_logits(model, x_var), y, t).mean()
    model.zero_grad(set_to_none=True)
    margin.backward()
    grad = x_var.grad
    if grad is None:
        raise RuntimeError("Cannot compute gradient for coordinate selection")
    g = grad.detach().abs().flatten()
    k = min(k, g.numel())
    idx = torch.topk(g, k=k).indices.cpu().numpy()
    return idx


def choose_coordinates_from_spsa(
    model: nn.Module,
    x: torch.Tensor,
    y: int,
    t: int,
    k: int,
    samples: int,
    delta: float,
    clip_min: float,
    clip_max: float,
) -> np.ndarray:
    grad = torch.zeros_like(x)
    for _ in range(samples):
        u = torch.randint_like(x, low=0, high=2, dtype=torch.int64).float() * 2.0 - 1.0
        x_plus = torch.clamp(x + delta * u, clip_min, clip_max)
        x_minus = torch.clamp(x - delta * u, clip_min, clip_max)
        m_plus = margin_value(model, x_plus, y, t)
        m_minus = margin_value(model, x_minus, y, t)
        grad += ((m_plus - m_minus) / (2.0 * delta)) * u
    grad /= float(samples)
    g = grad.detach().abs().flatten()
    k = min(k, g.numel())
    idx = torch.topk(g, k=k).indices.cpu().numpy()
    return idx


def monomial_exponents(dim: int, max_degree: int) -> List[Exponent]:
    out: List[Exponent] = []

    def rec(pos: int, left: int, prefix: List[int]) -> None:
        if pos == dim - 1:
            out.append(tuple(prefix + [left]))
            return
        for v in range(left + 1):
            rec(pos + 1, left - v, prefix + [v])

    for degree in range(max_degree + 1):
        rec(0, degree, [])
    return out


def eval_monomials(points: np.ndarray, exps: Sequence[Exponent]) -> np.ndarray:
    n = points.shape[0]
    phi = np.ones((n, len(exps)), dtype=np.float64)
    for i, e in enumerate(exps):
        term = np.ones(n, dtype=np.float64)
        for j, pwr in enumerate(e):
            if pwr:
                term *= points[:, j] ** pwr
        phi[:, i] = term
    return phi


def fit_polynomial(points: np.ndarray, values: np.ndarray, degree: int) -> Tuple[List[Exponent], np.ndarray]:
    exps = monomial_exponents(points.shape[1], degree)
    phi = eval_monomials(points, exps)
    coeffs, *_ = np.linalg.lstsq(phi, values, rcond=None)
    return exps, coeffs


def poly_to_dict(exps: Sequence[Exponent], coeffs: Sequence[float]) -> Poly:
    out: Poly = {}
    for e, c in zip(exps, coeffs):
        if abs(float(c)) > 1e-14:
            out[e] = float(c)
    return out


def poly_eval(points: np.ndarray, exps: Sequence[Exponent], coeffs: np.ndarray) -> np.ndarray:
    return eval_monomials(points, exps) @ coeffs


def poly_lipschitz_inf_bound(poly: Poly, eps: np.ndarray) -> float:
    dim = len(eps)
    partial_bounds = [0.0 for _ in range(dim)]
    for exp, coef_obj in poly.items():
        coef = float(coef_obj)
        for j in range(dim):
            p = exp[j]
            if p == 0:
                continue
            term = abs(coef) * p
            for m, em in enumerate(exp):
                pow_use = em - 1 if m == j else em
                if pow_use:
                    term *= float(eps[m]) ** pow_use
            partial_bounds[j] += term
    return float(sum(partial_bounds))


def poly_add(a: Poly, b: Poly, alpha: float = 1.0) -> Poly:
    out: Poly = dict(a)
    for e, c in b.items():
        out[e] = out.get(e, 0.0) + alpha * c
    return out


def poly_mul(a: Poly, b: Poly) -> Poly:
    out: Poly = {}
    for ea, ca in a.items():
        for eb, cb in b.items():
            e = tuple(x + y for x, y in zip(ea, eb))
            out[e] = out.get(e, 0.0) + ca * cb
    return out


def gram_poly_expr(Q: cp.Expression, basis: Sequence[Exponent]) -> Poly:
    out: Poly = {}
    n = len(basis)
    for i in range(n):
        ei = basis[i]
        for j in range(i, n):
            ej = basis[j]
            exp = tuple(a + b for a, b in zip(ei, ej))
            factor = 1.0 if i == j else 2.0
            out[exp] = out.get(exp, 0.0) + factor * Q[i, j]
    return out


def sos_lower_bound_box(
    poly_coeff: Poly,
    dim: int,
    eps: np.ndarray,
    multiplier_degree: int,
) -> Tuple[Optional[float], str]:
    max_deg = 0
    for e in poly_coeff.keys():
        max_deg = max(max_deg, sum(e))
    q_deg = max(1, math.ceil(max_deg / 2))
    q_basis = monomial_exponents(dim, q_deg)
    q_size = len(q_basis)
    Q = cp.Variable((q_size, q_size), symmetric=True)
    constraints = [Q >> 0]
    q_poly = gram_poly_expr(Q, q_basis)

    s_basis = monomial_exponents(dim, max(1, multiplier_degree // 2))
    s_size = len(s_basis)
    s_polys: List[Poly] = []
    for _ in range(dim):
        S = cp.Variable((s_size, s_size), symmetric=True)
        constraints.append(S >> 0)
        s_polys.append(gram_poly_expr(S, s_basis))

    gamma = cp.Variable()
    lhs: Poly = dict(poly_coeff)
    zero = tuple([0] * dim)
    lhs[zero] = lhs.get(zero, 0.0) - gamma

    for j in range(dim):
        e2 = [0] * dim
        e2[j] = 2
        g = {zero: float(eps[j]) ** 2, tuple(e2): -1.0}
        lhs = poly_add(lhs, poly_mul(s_polys[j], g), alpha=-1.0)

    all_exps = set(lhs.keys()) | set(q_poly.keys())
    for e in all_exps:
        constraints.append(lhs.get(e, 0.0) == q_poly.get(e, 0.0))

    prob = cp.Problem(cp.Maximize(gamma), constraints)
    attempts = [
        ("SCS", {"solver": cp.SCS, "eps": 1e-5, "max_iters": 200000, "verbose": False}),
        ("CLARABEL", {"solver": cp.CLARABEL, "verbose": False}),
    ]
    last_status = "not_solved"
    for solver_name, kwargs in attempts:
        try:
            prob.solve(**kwargs)
        except Exception as exc:
            last_status = f"{solver_name}_error:{exc}"
            continue
        last_status = f"{solver_name}:{prob.status}"
        if (
            prob.status in ("optimal", "optimal_inaccurate")
            and gamma.value is not None
            and np.isfinite(gamma.value)
        ):
            return float(gamma.value), last_status
    return None, last_status


def build_grid_box(eps_vec: np.ndarray, points_each_dim: int) -> np.ndarray:
    axes = []
    for e in eps_vec:
        axes.append(np.linspace(-float(e), float(e), points_each_dim))
    cart = list(itertools.product(*axes))
    return np.asarray(cart, dtype=np.float64)


def flatten_tensor(x: torch.Tensor) -> torch.Tensor:
    return x.reshape(-1)


def subspace_margin_values(
    model: nn.Module,
    x0: torch.Tensor,
    y: int,
    t: int,
    coords: np.ndarray,
    z_points: np.ndarray,
) -> np.ndarray:
    base_flat = flatten_tensor(x0.detach().cpu()).numpy().astype(np.float64)
    vals = np.zeros((z_points.shape[0],), dtype=np.float64)
    for i, z in enumerate(z_points):
        x_flat = base_flat.copy()
        x_flat[coords] += z
        x_t = torch.from_numpy(x_flat).to(x0.device, dtype=x0.dtype).reshape_as(x0)
        vals[i] = margin_value(model, x_t, y, t)
    return vals


def main() -> None:
    args = parse_args()
    device = torch.device(args.device)
    torch.set_grad_enabled(True)

    model = load_model(args).to(device)
    x0 = load_input_tensor(args.input_path, args.device)

    logits0 = model_logits(model, x0)
    pred0 = int(logits0.argmax(dim=1).item())
    y = int(args.true_label) if args.true_label is not None else pred0
    t = pick_target(logits0, y, args.target_label)

    base_margin = float(margin_from_logits(logits0, y, t).item())
    print(f"[info] base prediction={pred0}, true_label={y}, target_label={t}, margin={base_margin:.6f}")

    attack_method = args.attack
    adv_margin = base_margin
    adv_pred = pred0
    attack_found = False

    if args.attack != "none":
        if args.attack in ("auto", "pgd"):
            try:
                adv_x, adv_margin = pgd_attack(
                    model=model,
                    x0=x0,
                    y=y,
                    t=t,
                    eps=args.eps,
                    steps=args.attack_steps,
                    alpha=args.attack_step_size,
                    clip_min=args.clip_min,
                    clip_max=args.clip_max,
                )
                attack_method = "pgd"
            except Exception as exc:
                if args.attack == "pgd":
                    raise
                print(f"[warn] PGD failed ({exc}), falling back to SPSA")
                adv_x, adv_margin = spsa_attack(
                    model=model,
                    x0=x0,
                    y=y,
                    t=t,
                    eps=args.eps,
                    steps=args.attack_steps,
                    alpha=args.attack_step_size,
                    samples=args.spsa_samples,
                    delta=args.spsa_delta,
                    clip_min=args.clip_min,
                    clip_max=args.clip_max,
                )
                attack_method = "spsa"
        else:
            adv_x, adv_margin = spsa_attack(
                model=model,
                x0=x0,
                y=y,
                t=t,
                eps=args.eps,
                steps=args.attack_steps,
                alpha=args.attack_step_size,
                samples=args.spsa_samples,
                delta=args.spsa_delta,
                clip_min=args.clip_min,
                clip_max=args.clip_max,
            )
            attack_method = "spsa"

        adv_pred = int(model_logits(model, adv_x).argmax(dim=1).item())
        attack_found = adv_margin <= 0.0
        print(f"[info] attack={attack_method}, margin={adv_margin:.6f}, pred={adv_pred}")
        if attack_found:
            print("[result] adversarial sample found in epsilon-ball; model is not robust.")

    flat0 = flatten_tensor(x0.detach().cpu()).numpy()
    k = int(max(1, args.subspace_dim))
    try:
        coords = choose_coordinates_from_gradient(model, x0, y, t, k)
    except Exception as exc:
        print(f"[warn] gradient coordinate selection failed: {exc}; trying SPSA sensitivity")
        try:
            coords = choose_coordinates_from_spsa(
                model=model,
                x=x0,
                y=y,
                t=t,
                k=k,
                samples=max(8, args.spsa_samples),
                delta=max(1e-6, args.spsa_delta),
                clip_min=args.clip_min,
                clip_max=args.clip_max,
            )
        except Exception as exc2:
            print(f"[warn] SPSA coordinate selection failed: {exc2}; using first k coordinates")
            coords = np.arange(min(k, flat0.size))
    if len(coords) == 0:
        raise RuntimeError("No coordinates selected for SOS subspace")

    eps_vec = np.full((len(coords),), args.eps, dtype=np.float64)
    for i, c in enumerate(coords):
        up = args.clip_max - float(flat0[c])
        down = float(flat0[c]) - args.clip_min
        eps_vec[i] = min(eps_vec[i], up, down)
        if eps_vec[i] <= 0:
            raise RuntimeError(
                f"Selected coordinate {int(c)} has zero available perturbation under clipping constraints."
            )

    z_grid = build_grid_box(eps_vec, args.grid_points)
    if z_grid.shape[0] > 10000:
        raise ValueError(
            f"Grid too large ({z_grid.shape[0]} points). Reduce --grid-points or --subspace-dim."
        )

    m_vals = subspace_margin_values(model, x0, y, t, coords, z_grid)
    exps, coeffs = fit_polynomial(z_grid, m_vals, args.poly_degree)
    poly = poly_to_dict(exps, coeffs)
    poly_vals = poly_eval(z_grid, exps, coeffs)
    residual = m_vals - poly_vals

    gamma_poly_lb, sos_status = sos_lower_bound_box(
        poly_coeff=poly,
        dim=len(coords),
        eps=eps_vec,
        multiplier_degree=args.sos_multiplier_degree,
    )

    h = 0.0
    if args.grid_points > 1:
        h = float(np.max(eps_vec / (args.grid_points - 1)))

    l_poly = poly_lipschitz_inf_bound(poly, eps_vec)
    rigorous = args.margin_lipschitz_inf is not None
    if rigorous:
        l_margin = float(args.margin_lipschitz_inf)
    else:
        l_margin = 0.0
        print(
            "[warn] --margin-lipschitz-inf not provided; residual correction is heuristic only "
            "(not a deterministic proof)."
        )

    residual_lb: Optional[float] = None
    certified_lb: Optional[float] = None
    robust_subspace = False
    if gamma_poly_lb is None:
        print(
            "[warn] SOS solver did not return a finite lower bound "
            f"(status={sos_status}); certification is inconclusive."
        )
    else:
        residual_lb = float(np.min(residual) - (l_margin + l_poly) * h)
        certified_lb = float(gamma_poly_lb + residual_lb)
        robust_subspace = certified_lb > 0.0
        print(
            "[info] SOS(poly) LB={:.6f}, residual LB={:.6f}, certified subspace LB={:.6f}".format(
                gamma_poly_lb, residual_lb, certified_lb
            )
        )

    if gamma_poly_lb is None:
        print("[result] SOS stage inconclusive (no valid SDP optimum returned).")
    elif rigorous:
        if robust_subspace:
            print("[result] deterministic certificate: robust in verified coordinate subspace.")
        else:
            print("[result] deterministic certificate failed: cannot prove robustness in subspace.")
    else:
        print("[result] heuristic bound only (no rigorous Lipschitz constant supplied).")

    note = (
        "Attack verifies full input-space vulnerability if found. "
        "SOS certificate applies to the selected coordinate subspace only."
    )
    result = VerificationResult(
        attack_method=attack_method,
        attack_found=bool(attack_found),
        attack_margin=float(adv_margin),
        attack_label=int(adv_pred),
        base_label=int(pred0),
        target_label=int(t),
        sos_status=str(sos_status),
        sos_polynomial_lb=None if gamma_poly_lb is None else float(gamma_poly_lb),
        residual_lb=residual_lb,
        certified_lb_subspace=certified_lb,
        rigorous_subspace_certificate=bool(rigorous),
        robust_in_verified_subspace=bool(robust_subspace),
        note=note,
    )

    if args.output_json:
        out_path = Path(args.output_json)
        out_path.write_text(json.dumps(asdict(result), indent=2), encoding="utf-8")
        print(f"[info] wrote {out_path}")
    else:
        print(json.dumps(asdict(result), indent=2))


if __name__ == "__main__":
    main()
