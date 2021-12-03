
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
  lookTableIndex = lookTableIndex + 600;
  if (lookTableIndex < 0 || fxp_to_float(input) < -6.0f) {
    return fxp_float_to_fxp(0.0f);
  } else if (lookTableIndex >= 1200 || fxp_to_float(input) > 6.0f ) {
    return fxp_float_to_fxp(1.0f);
  } else {
    float value = lookup[lookTableIndex];
    return fxp_float_to_fxp(value);
  }
}

float nondet_float();

fxp_t performf(float *weights, unsigned int weights_size, fxp_t *inputs,
               unsigned int input_size, float bias) {
  if (weights_size != input_size) {
    return 0;
  }

  unsigned size = weights_size;
  fxp_t result = 0;

  for (unsigned int i = 0; i < size; ++i) {
    fxp_t w = fxp_float_to_fxp(weights[i]);
    result = fxp_add(result, fxp_mult(w, inputs[i]));
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
  float norm = 0.00392156863;

  int x0 = nondet_int();
  __ESBMC_assume((x0 >= 215) && (x0 <= 255));
  int x1 = nondet_int();
  __ESBMC_assume((x1 >= 215) && (x1 <= 255));
  int x2 = nondet_int();
  __ESBMC_assume((x2 >= 215) && (x2 <= 255));
  int x3 = nondet_int();
  __ESBMC_assume((x3 >= 215) && (x3 <= 255));
  int x4 = nondet_int();
  __ESBMC_assume((x4 >= 215) && (x4 <= 255));
  int x5 = nondet_int();
  __ESBMC_assume((x5 >= 215) && (x5 <= 255));
  int x6 = nondet_int();
  __ESBMC_assume((x6 >= 0) && (x6 <= 40));
  int x7 = nondet_int();
  __ESBMC_assume((x7 >= 0) && (x7 <= 40));
  int x8 = nondet_int();
  __ESBMC_assume((x8 >= 0) && (x8 <= 40));
  int x9 = nondet_int();
  __ESBMC_assume((x9 >= 215) && (x9 <= 255));
  int x10 = nondet_int();
  __ESBMC_assume((x10 >= 215) && (x10 <= 255));
  int x11 = nondet_int();
  __ESBMC_assume((x11 >= 215) && (x11 <= 255));
  int x12 = nondet_int();
  __ESBMC_assume((x12 >= 215) && (x12 <= 255));
  int x13 = nondet_int();
  __ESBMC_assume((x13 >= 215) && (x13 <= 255));
  int x14 = nondet_int();
  __ESBMC_assume((x14 >= 215) && (x14 <= 255));
  int x15 = nondet_int();
  __ESBMC_assume((x15 >= 215) && (x15 <= 255));
  int x16 = nondet_int();
  __ESBMC_assume((x16 >= 0) && (x16 <= 40));
  int x17 = nondet_int();
  __ESBMC_assume((x17 >= 0) && (x17 <= 40));
  int x18 = nondet_int();
  __ESBMC_assume((x18 >= 0) && (x18 <= 40));
  int x19 = nondet_int();
  __ESBMC_assume((x19 >= 215) && (x19 <= 255));
  int x20 = nondet_int();
  __ESBMC_assume((x20 >= 215) && (x20 <= 255));
  int x21 = nondet_int();
  __ESBMC_assume((x21 >= 0) && (x21 <= 40));
  int x22 = nondet_int();
  __ESBMC_assume((x22 >= 0) && (x22 <= 40));
  int x23 = nondet_int();
  __ESBMC_assume((x23 >= 0) && (x23 <= 40));
  int x24 = nondet_int();
  __ESBMC_assume((x24 >= 215) && (x24 <= 255));

  fxp_t i[25];
  i[0] = fxp_float_to_fxp(((float) x0) * norm);
  i[1] = fxp_float_to_fxp(((float) x1) * norm);
  i[2] = fxp_float_to_fxp(((float) x2) * norm);
  i[3] = fxp_float_to_fxp(((float) x3) * norm);
  i[4] = fxp_float_to_fxp(((float) x4) * norm);
  i[5] = fxp_float_to_fxp(((float) x5) * norm);
  i[6] = fxp_float_to_fxp(((float) x6) * norm);
  i[7] = fxp_float_to_fxp(((float) x7) * norm);
  i[8] = fxp_float_to_fxp(((float) x8) * norm);
  i[9] = fxp_float_to_fxp(((float) x9) * norm);
  i[10] = fxp_float_to_fxp(((float) x10) * norm);
  i[11] = fxp_float_to_fxp(((float) x11) * norm);
  i[12] = fxp_float_to_fxp(((float) x12) * norm);
  i[13] = fxp_float_to_fxp(((float) x13) * norm);
  i[14] = fxp_float_to_fxp(((float) x14) * norm);
  i[15] = fxp_float_to_fxp(((float) x15) * norm);
  i[16] = fxp_float_to_fxp(((float) x16) * norm);
  i[17] = fxp_float_to_fxp(((float) x17) * norm);
  i[18] = fxp_float_to_fxp(((float) x18) * norm);
  i[19] = fxp_float_to_fxp(((float) x19) * norm);
  i[20] = fxp_float_to_fxp(((float) x20) * norm);
  i[21] = fxp_float_to_fxp(((float) x21) * norm);
  i[22] = fxp_float_to_fxp(((float) x22) * norm);
  i[23] = fxp_float_to_fxp(((float) x23) * norm);
  i[24] = fxp_float_to_fxp(((float) x24) * norm);

  fxp_t layer1[5];

  float w10[25] = {0.207958f,  0.044154f,  -4.232451f, 0.745003f,  -1.340141f,
                   -1.486082f, -1.869501f, -1.486536f, -0.624669f, 1.210342f,
                   -4.070879f, -0.600616f, -5.623879f, -3.807880f, 1.362974f,
                   1.334723f,  -2.295741f, 3.483194f,  -4.446033f, 0.078016f,
                   4.587534f,  -0.206197f, 0.977980f,  1.286488f,  0.772414f};
  float b10 = -0.095517f;
  fxp_t r10 = performf(w10, 25, i, 25, b10);
  layer1[0] = r10;
  __ESBMC_assume((layer1[0] >= fxp_float_to_fxp(9.99999997475e-07)) &&
                 (layer1[0] <= fxp_float_to_fxp(0.0012379999971)));

  float w11[25] = {-1.486082f, -1.869501f, -1.486536f, -0.624669f, 1.210342f,
                   -4.070879f, -0.600616f, -5.623879f, -3.807880f, 1.362974f,
                   1.334723f,  -2.295741f, 3.483194f,  -4.446033f, 0.078016f,
                   4.587534f,  -0.206197f, 0.977980f,  1.286488f,  0.772414f,
                   -0.095517f, -0.237044f, 1.996482f,  2.389851f,  -0.082591f};
  float b11 = 2.035967f;
  fxp_t r11 = performf(w11, 25, i, 25, b11);
  layer1[1] = r11;
  __ESBMC_assume((layer1[1] >= fxp_float_to_fxp(0.00523100001737)) &&
                 (layer1[1] <= fxp_float_to_fxp(0.884115993977)));

  float w12[25] = {-4.070879f, -0.600616f, -5.623879f, -3.807880f, 1.362974f,
                   1.334723f,  -2.295741f, 3.483194f,  -4.446033f, 0.078016f,
                   4.587534f,  -0.206197f, 0.977980f,  1.286488f,  0.772414f,
                   -0.095517f, -0.237044f, 1.996482f,  2.389851f,  -0.082591f,
                   2.035967f,  -1.636084f, -2.794725f, -1.262111f, 6.436423f};
  float b12 = -0.922335f;
  fxp_t r12 = performf(w12, 25, i, 25, b12);
  layer1[2] = r12;
  __ESBMC_assume((layer1[2] >= fxp_float_to_fxp(0.184672996402)) &&
                 (layer1[2] <= fxp_float_to_fxp(0.999059975147)));

  float w13[25] = {1.334723f,  -2.295741f, 3.483194f,  -4.446033f, 0.078016f,
                   4.587534f,  -0.206197f, 0.977980f,  1.286488f,  0.772414f,
                   -0.095517f, -0.237044f, 1.996482f,  2.389851f,  -0.082591f,
                   2.035967f,  -1.636084f, -2.794725f, -1.262111f, 6.436423f,
                   -0.922335f, -2.358342f, -0.142077f, -4.891456f, -2.749024f};
  float b13 = 0.042693f;
  fxp_t r13 = performf(w13, 25, i, 25, b13);
  layer1[3] = r13;
  __ESBMC_assume((layer1[3] >= fxp_float_to_fxp(0.998665988445)) &&
                 (layer1[3] <= fxp_float_to_fxp(0.999998986721)));

  float w14[25] = {4.587534f,  -0.206197f, 0.977980f,  1.286488f,  0.772414f,
                   -0.095517f, -0.237044f, 1.996482f,  2.389851f,  -0.082591f,
                   2.035967f,  -1.636084f, -2.794725f, -1.262111f, 6.436423f,
                   -0.922335f, -2.358342f, -0.142077f, -4.891456f, -2.749024f,
                   0.042693f,  0.455576f,  0.108683f,  0.724179f,  0.679236f};
  float b14 = 2.473645f;
  fxp_t r14 = performf(w14, 25, i, 25, b14);
  layer1[4] = r14;
  __ESBMC_assume((layer1[4] >= fxp_float_to_fxp(0.99669200182)) &&
                 (layer1[4] <= fxp_float_to_fxp(0.999993979931)));

  fxp_t layer2[4];

  float w20[5] = {-8.332183f, -3.040542f, 3.758681f, -4.049425f, 3.050940f};
  float b20 = -0.894016f;
  fxp_t r20 = performf(w20, 5, layer1, 5, b20);
  layer2[0] = r20;
  __ESBMC_assume((layer2[0] >= fxp_float_to_fxp(0.0197240002453)) &&
                 (layer2[0] <= fxp_float_to_fxp(0.864362001419)));

  float w21[5] = {-0.894016f, -1.446080f, 6.029086f, 5.837497f, -1.206468f};
  float b21 = -2.927471f;
  fxp_t r21 = performf(w21, 5, layer1, 5, b21);
  layer2[1] = r21;
  __ESBMC_assume((layer2[1] >= fxp_float_to_fxp(0.821860015392)) &&
                 (layer2[1] <= fxp_float_to_fxp(0.999557971954)));

  float w22[5] = {-2.927471f, -4.145836f, 1.903108f, 3.409983f, 0.191798f};
  float b22 = -4.116436f;
  fxp_t r22 = performf(w22, 5, layer1, 5, b22);
  layer2[2] = r22;
  __ESBMC_assume((layer2[2] >= fxp_float_to_fxp(0.0211030002683)) &&
                 (layer2[2] <= fxp_float_to_fxp(0.796409010887)));

  float w23[5] = {-4.116436f, -7.171240f, 2.687001f, -1.662798f, 3.236128f};
  float b23 = 4.911131f;
  fxp_t r23 = performf(w23, 5, layer1, 5, b23);
  layer2[3] = r23;
  __ESBMC_assume((layer2[3] >= fxp_float_to_fxp(0.651127994061)) &&
                 (layer2[3] <= fxp_float_to_fxp(0.999891996384)));

  fxp_t layer3[5];

  float w30[4] = {5.578862f, 5.071550f, 6.798682f, -0.259254f};
  float b30 = -12.202182f;
  fxp_t r30 = performf(w30, 4, layer2, 4, b30);
  layer3[0] = r30;
  __ESBMC_assume((layer3[0] >= fxp_float_to_fxp(0.000322999985656)) &&
                 (layer3[0] <= fxp_float_to_fxp(0.949549973011)));
  int r = 0;

  float w31[4] = {10.556403f, -9.337465f, -0.761154f, 2.529015f};
  float b31 = -8.269168f;
  fxp_t r31 = performf(w31, 4, layer2, 4, b31);
  layer3[1] = r31;
  __ESBMC_assume((layer3[1] >= fxp_float_to_fxp(0)) &&
                 (layer3[1] <= fxp_float_to_fxp(0.0133079998195)));
  if (fxp_to_float(layer3[1]) > fxp_to_float(layer3[r]))
    r = 1;

  float w32[4] = {-11.307169f, -8.039979f, -2.417081f, 9.576913f};
  float b32 = -6.212107f;
  fxp_t r32 = performf(w32, 4, layer2, 4, b32);
  layer3[2] = r32;
  __ESBMC_assume((layer3[2] >= fxp_float_to_fxp(0)) &&
                 (layer3[2] <= fxp_float_to_fxp(0.0288319997489)));
  if (fxp_to_float(layer3[2]) > fxp_to_float(layer3[r]))
    r = 2;

  float w33[4] = {-2.461208f, -6.955447f, -1.412636f, -12.198679f};
  float b33 = 3.503773f;
  fxp_t r33 = performf(w33, 4, layer2, 4, b33);
  layer3[3] = r33;
  __ESBMC_assume((layer3[3] >= fxp_float_to_fxp(0)) &&
                 (layer3[3] <= fxp_float_to_fxp(3.60000012734e-05)));
  if (fxp_to_float(layer3[3]) > fxp_to_float(layer3[r]))
    r = 3;

  float w34[4] = {-2.784967f, 8.339398f, -1.883454f, -9.724778f};
  float b34 = -3.755674f;
  fxp_t r34 = performf(w34, 4, layer2, 4, b34);
  layer3[4] = r34;
  __ESBMC_assume((layer3[4] >= fxp_float_to_fxp(2.70000000455e-05)) &&
                 (layer3[4] <= fxp_float_to_fxp(0.136343002319)));
  if (fxp_to_float(layer3[4]) > fxp_to_float(layer3[r]))
    r = 4;
  __ESBMC_assert(r == 0, "Classification is not a 0 anymore.");
  //@assert(r == 0);
}
