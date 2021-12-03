#include "../../../dsverifier/definitions.h"
#include "../../../dsverifier/fixed_point.h"
#include "../../../dsverifier/initialization.h"
#include "../../../utils.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

fxp_t check_activation(fxp_t input) {
  fxp_t factor0 = fxp_float_to_fxp(100.0f);
  fxp_t prod0 = fxp_mult(input, factor0);
  int lookTableIndex = fxp_to_int(prod0);
  lookTableIndex = lookTableIndex + 400;
  if (lookTableIndex < 0) {
    return fxp_float_to_fxp(-1.0f);
  } else if (lookTableIndex >= 800) {
    return fxp_float_to_fxp(1.0f);
  } else {
    float value = lookuptanh[lookTableIndex];
    return fxp_float_to_fxp(value);
  }
  return fxp_float_to_fxp(1.0f);
}

float nondet_float();

fxp_t perform(float *weights, unsigned int weights_size, float *inputs,
              unsigned int input_size, float bias) {
  if (weights_size != input_size) {
    return 0;
  }

  unsigned size = weights_size;
  fxp_t result = 0;

  for (unsigned int i = 0; i < size; ++i) {
    fxp_t w = fxp_float_to_fxp(weights[i]);
    fxp_t i = fxp_float_to_fxp(inputs[i]);
    result = fxp_add(result, fxp_mult(w, i));
  }

  fxp_t b = fxp_float_to_fxp(bias);
  result = fxp_add(result, b);

  result = check_activation(result);

  return result;
}

int main() {
  int int_bits = 3;
  int frac_bits = 4;

#ifdef INT_BITS
  int_bits = INT_BITS;
#endif

#ifdef FRAC_BITS
  frac_bits = FRAC_BITS;
#endif

  initialization(int_bits, frac_bits);

  float norm = (float)1 / (float)100;
  int x0 = nondet_int();
  __ESBMC_assume((x0 >= 294) && (x0 <= 686));
  int x1 = nondet_int();
  __ESBMC_assume((x1 >= 180) && (x1 <= 420));
  int x2 = nondet_int();
  __ESBMC_assume((x2 >= 84) && (x2 <= 196));
  int x3 = nondet_int();
  __ESBMC_assume((x3 >= 12) && (x3 <= 28));

  float i[4];
  i[0] = x0 * norm;
  i[1] = x1 * norm;
  i[2] = x2 * norm;
  i[3] = x3 * norm;

  int r = 0;

  fxp_t layer1[7];

  float w0[4] = {-0.259293f, -0.147145f, -0.070675f, -0.396830f};
  float b0 = -0.019240f;
  fxp_t r0 = perform(w0, 4, i, 4, b0);
  layer1[0] = r0;
  __ESBMC_assume((layer1[0] >= fxp_float_to_fxp(-0.905201971531)) &&
                 (layer1[0] <= fxp_float_to_fxp(-0.664650976658)));

  float w1[4] = {0.195045f, 0.474309f, -0.148397f, -0.079880f};
  float b1 = -0.016677f;
  fxp_t r1 = perform(w1, 4, i, 4, b1);
  layer1[1] = r1;
  __ESBMC_assume((layer1[1] >= fxp_float_to_fxp(0.677024006844)) &&
                 (layer1[1] <= fxp_float_to_fxp(0.912406027317)));

  float w2[4] = {0.461122f, -0.476375f, 0.248677f, -0.553619f};
  float b2 = -0.017581f;
  fxp_t r2 = perform(w2, 4, i, 4, b2);
  layer1[2] = r2;
  __ESBMC_assume((layer1[2] >= fxp_float_to_fxp(-0.550779998302)) &&
                 (layer1[2] <= fxp_float_to_fxp(0.648207008839)));

  float w3[4] = {-0.324553f, -0.542259f, 1.332844f, 0.068103f};
  float b3 = -0.118043f;
  fxp_t r3 = perform(w3, 4, i, 4, b3);
  layer1[3] = r3;
  __ESBMC_assume((layer1[3] >= fxp_float_to_fxp(-0.759697973728)) &&
                 (layer1[3] <= fxp_float_to_fxp(0.674306988716)));

  float w4[4] = {0.405520f, 0.221390f, -0.391765f, 0.337188f};
  float b4 = -0.018501f;
  fxp_t r4 = perform(w4, 4, i, 4, b4);
  layer1[4] = r4;
  __ESBMC_assume((layer1[4] >= fxp_float_to_fxp(0.356961995363)) &&
                 (layer1[4] <= fxp_float_to_fxp(0.897283017635)));

  float w5[4] = {0.651486f, -0.367779f, -0.224814f, -0.037613f};
  float b5 = 0.077769f;
  fxp_t r5 = perform(w5, 4, i, 4, b5);
  layer1[5] = r5;
  __ESBMC_assume((layer1[5] >= fxp_float_to_fxp(-0.15347699821)) &&
                 (layer1[5] <= fxp_float_to_fxp(0.701678991318)));

  float w6[4] = {0.981716f, 0.727559f, -1.363340f, -1.402215f};
  float b6 = 0.674765f;
  fxp_t r6 = perform(w6, 4, i, 4, b6);
  layer1[6] = r6;
  __ESBMC_assume((layer1[6] >= fxp_float_to_fxp(0.0899569988251)) &&
                 (layer1[6] <= fxp_float_to_fxp(0.99870300293)));

  fxp_t layer2[3];

  float w20[7] = {-0.228491f, -0.172476f, -0.608707f, -1.003181f,
                  0.519332f,  0.686671f,  -0.028897f};
  float b20 = 0.342278f;
  float i20[7];
  fxp_to_float_array(i20, layer1, 7);
  fxp_t r20 = perform(w20, 7, i20, 7, b20);
  layer2[0] = r20;
  __ESBMC_assume((layer2[0] >= fxp_float_to_fxp(-0.593531012535)) &&
                 (layer2[0] <= fxp_float_to_fxp(0.985930025578)));

  float w21[7] = {0.782173f,  -0.306530f, -0.409494f, 0.967240f,
                  -0.508822f, 0.187891f,  0.998079f};
  float b21 = 0.046970f;
  float i21[7];
  fxp_to_float_array(i21, layer1, 7);
  fxp_t r21 = perform(w21, 7, i21, 7, b21);
  layer2[1] = r21;
  __ESBMC_assume((layer2[1] >= fxp_float_to_fxp(-0.803566992283)) &&
                 (layer2[1] <= fxp_float_to_fxp(0.982749998569)));

  float left = fxp_to_float(layer2[1]);
  float right = fxp_to_float(layer2[r]);

  if (left > right) {
    r = 1;
  }

  float w22[7] = {-0.182530f, 0.064892f, 0.407630f, -0.205405f,
                  -0.140221f, 0.204790f, -0.901339f};
  float b22 = 0.131860f;
  float i22[7];
  fxp_to_float_array(i22, layer1, 7);
  fxp_t r22 = perform(w22, 7, i22, 7, b22);
  layer2[2] = r22;
  __ESBMC_assume((layer2[2] >= fxp_float_to_fxp(-0.808713972569)) &&
                 (layer2[2] <= fxp_float_to_fxp(0.657899022102)));

  left = fxp_to_float(layer2[2]);
  right = fxp_to_float(layer2[r]);

  if (left > right) {
    r = 2;
  }

  __ESBMC_assert(r == 0, "Classification is not a 0 anymore.");
}
