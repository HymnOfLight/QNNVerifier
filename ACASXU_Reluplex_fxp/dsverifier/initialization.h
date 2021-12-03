/**
 * DSVerifier - Digital Systems Verifier
 *
 * Federal University of Amazonas - UFAM
 *
 * Authors:       Hussama Ismail <hussamaismail@gmail.com>
 *                Iury Bessa     <iury.bessa@gmail.com>
 *                Renato Abreu   <renatobabreu@yahoo.com.br>
 *
 * ------------------------------------------------------
 *
 * initialize internal variables
 *
 * ------------------------------------------------------
 */
#ifndef DSVERIFIER_CORE_INITIALIZATION_H
#define DSVERIFIER_CORE_INITIALIZATION_H

#include "definitions.h"
#include "fixed_point.h"
#include <float.h>

/** function to set the necessary parameters to DSVerifier FXP library */
void initialization(int intb, int fracb)
{
  int_bits = intb;
  frac_bits = fracb;

#if (ARITHMETIC == FIXEDBV)
  if (frac_bits >= FXP_WIDTH)
  {
    printf("frac_bits must be less than the word-width!");
    assert(0);
  }
  if (int_bits >= FXP_WIDTH - frac_bits)
  {
    printf("int_bits must be less than the word-width subtracted by "
           "the precision!");
    assert(0);
  }

  if (frac_bits >= 31)
  {
    _fxp_one = 0x7fffffff;
  }
  else
  {
    _fxp_one = (0x00000001 << frac_bits);
  }

  _fxp_half = (0x00000001 << (frac_bits - 1));
  _fxp_minus_one = -(0x00000001 << frac_bits);
  _fxp_min = -(0x00000001 << (frac_bits + int_bits - 1));
  _fxp_max = (0x00000001 << (frac_bits + int_bits - 1)) - 1;
  _fxp_fmask = ((((int32_t)1) << frac_bits) - 1);
  _fxp_imask = ((0x80000000) >> (FXP_WIDTH - frac_bits - 1));

  _dbl_min = _fxp_min;
  _dbl_min /= (1 << frac_bits);
  _dbl_max = _fxp_max;
  _dbl_max /= (1 << frac_bits);
#elif (ARITHMETIC == FLOATBV)
  _fp_min = FLT_MIN;
  _fp_max = FLT_MAX;
#endif

  /* check if the scale exists */
  if ((scale == 0) || (scale == 1))
  {
    scale = 1;
    return;
  }

  /** applying scale in dynamical range */
  if (min != 0)
  {
    min = min / scale;
  }

  if (max != 0)
  {
    max = max / scale;
  }
}
#endif // DSVERIFIER_CORE_INITIALIZATION_H
