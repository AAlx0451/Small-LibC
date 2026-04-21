/* This file has been put into the public domain by its author. */

#include <fenv.h>
#include <math.h>
#include <stdint.h>
#include <string.h>

int _Fpclassifyf(float f) {
    uint32_t bits;
    memcpy(&bits, &f, sizeof(float));

    /* IEEE 754 float: 1 bit sign, 8 bit exp, 23 bit mant */
    uint32_t exp = (bits >> 23) & 0xFF;
    uint32_t frac = bits & 0x7FFFFF;

    if(exp == 0) {
        return frac == 0 ? FP_ZERO : FP_SUBNORMAL;
    }
    if(exp == 0xFF) {
        return frac == 0 ? FP_INFINITE : FP_NAN;
    }
    return FP_NORMAL;
}
