#!/usr/bin/env python3
"""
Export torchvision quantized ResNet to TorchScript .pth.

Outputs:
1) TorchScript quantized model (.pth)
2) Example input tensor (.npy, optional .pt)
3) Metadata JSON (top1 prediction, logits shape, export config)

Example:
  python3 export_quant_resnet_torchscript.py \
    --arch resnet18 \
    --weights default \
    --output-model quant_resnet18_scripted.pth \
    --output-input-npy sample_input.npy \
    --output-meta export_meta.json
"""

from __future__ import annotations

import argparse
import json
import random
from pathlib import Path
from typing import Any, Dict, Optional

import numpy as np
import torch


SUPPORTED_ARCH = ("resnet18", "resnet50")


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="Export quantized torchvision ResNet as TorchScript .pth."
    )
    parser.add_argument("--arch", type=str, default="resnet18", choices=SUPPORTED_ARCH)
    parser.add_argument(
        "--weights",
        type=str,
        default="default",
        choices=("default", "none"),
        help="Use pretrained quantized weights or random initialization.",
    )
    parser.add_argument("--image-size", type=int, default=224)
    parser.add_argument("--seed", type=int, default=0)
    parser.add_argument("--device", type=str, default="cpu")

    parser.add_argument(
        "--output-model",
        type=str,
        default="quant_resnet18_scripted.pth",
        help="Output TorchScript file path.",
    )
    parser.add_argument(
        "--output-input-npy",
        type=str,
        default="sample_input.npy",
        help="Output example input (.npy).",
    )
    parser.add_argument(
        "--output-input-pt",
        type=str,
        default=None,
        help="Optional output example input (.pt).",
    )
    parser.add_argument(
        "--output-meta",
        type=str,
        default="export_meta.json",
        help="Output metadata JSON path.",
    )
    return parser.parse_args()


def seed_everything(seed: int) -> None:
    random.seed(seed)
    np.random.seed(seed)
    torch.manual_seed(seed)


def try_build_quantized_model(arch: str, use_default_weights: bool) -> torch.nn.Module:
    import torchvision.models.quantization as tq

    factory = getattr(tq, arch)
    kwargs: Dict[str, Any] = {"quantize": True}

    # torchvision new API (weights enum) and old API (pretrained bool) differ.
    # Try a sequence of compatible calls for robust export across versions.
    candidates = []
    if use_default_weights:
        enum_name = {
            "resnet18": "ResNet18_QuantizedWeights",
            "resnet50": "ResNet50_QuantizedWeights",
        }[arch]
        enum_cls = getattr(tq, enum_name, None)
        if enum_cls is not None and hasattr(enum_cls, "DEFAULT"):
            candidates.append({"weights": enum_cls.DEFAULT, **kwargs})
        candidates.append({"pretrained": True, **kwargs})
    else:
        candidates.append({"weights": None, **kwargs})
        candidates.append({"pretrained": False, **kwargs})

    last_err: Optional[Exception] = None
    for cand in candidates:
        try:
            model = factory(**cand)
            model.eval()
            return model
        except Exception as exc:  # pragma: no cover - compatibility fallback
            last_err = exc

    raise RuntimeError(
        f"Failed to build quantized {arch}. "
        f"Tried kwargs={candidates}, last_error={last_err}"
    )


def make_example_input(image_size: int, device: torch.device) -> torch.Tensor:
    # Use [0,1] range to match common normalized image tensor workflows.
    x = torch.rand(1, 3, image_size, image_size, device=device, dtype=torch.float32)
    return x


def export_torchscript(model: torch.nn.Module, example_input: torch.Tensor, output_path: Path) -> str:
    output_path.parent.mkdir(parents=True, exist_ok=True)

    mode_used = "script"
    try:
        scripted = torch.jit.script(model)
    except Exception:
        # Quantized graphs can be fragile for scripting depending on version.
        # Fallback to trace with an example input.
        scripted = torch.jit.trace(model, example_input, strict=False)
        mode_used = "trace"
    scripted.save(str(output_path))
    return mode_used


def save_input(example_input: torch.Tensor, npy_path: Path, pt_path: Optional[Path]) -> None:
    npy_path.parent.mkdir(parents=True, exist_ok=True)
    np.save(str(npy_path), example_input.detach().cpu().numpy())
    if pt_path is not None:
        pt_path.parent.mkdir(parents=True, exist_ok=True)
        torch.save(example_input.detach().cpu(), str(pt_path))


def main() -> None:
    args = parse_args()
    seed_everything(args.seed)

    if args.arch not in SUPPORTED_ARCH:
        raise ValueError(f"Unsupported arch {args.arch!r}, choose from {SUPPORTED_ARCH}")

    device = torch.device(args.device)
    if device.type != "cpu":
        print("[warn] quantized inference/export is usually CPU-only; continuing as requested.")

    model = try_build_quantized_model(args.arch, use_default_weights=(args.weights == "default"))
    model = model.to(device).eval()
    x = make_example_input(args.image_size, device)

    with torch.no_grad():
        logits = model(x)
        if isinstance(logits, (tuple, list)):
            logits = logits[0]
        top1 = int(torch.argmax(logits[0]).item())

    output_model = Path(args.output_model)
    output_npy = Path(args.output_input_npy)
    output_pt = Path(args.output_input_pt) if args.output_input_pt else None
    output_meta = Path(args.output_meta)

    ts_mode = export_torchscript(model, x, output_model)
    save_input(x, output_npy, output_pt)

    meta = {
        "arch": args.arch,
        "weights": args.weights,
        "image_size": args.image_size,
        "seed": args.seed,
        "device": args.device,
        "torch_version": torch.__version__,
        "torchvision_version": __import__("torchvision").__version__,
        "torchscript_export_mode": ts_mode,
        "output_model": str(output_model),
        "output_input_npy": str(output_npy),
        "output_input_pt": str(output_pt) if output_pt else None,
        "top1_label_on_sample": top1,
        "logits_shape": list(logits.shape),
    }
    output_meta.parent.mkdir(parents=True, exist_ok=True)
    output_meta.write_text(json.dumps(meta, indent=2), encoding="utf-8")

    print(f"[ok] exported model: {output_model}")
    print(f"[ok] exported input: {output_npy}")
    if output_pt is not None:
        print(f"[ok] exported input pt: {output_pt}")
    print(f"[ok] exported metadata: {output_meta}")
    print(f"[ok] sample top1 label: {top1}")
    print(f"[ok] torchscript mode: {ts_mode}")


if __name__ == "__main__":
    main()
