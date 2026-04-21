/* This file has been put into the public domain by its author. */

#include <fenv.h>
#include <math.h>
#include <stdint.h>
#include <string.h>

int _Fpclassify(double d) {
    uint64_t bits;
    memcpy(&bits, &d, sizeof(double));

    /* IEEE 754 double: 1 bit sign, 11 bit exp, 52 bit mant */
    uint32_t exp = (bits >> 52) & 0x7FF;
    uint64_t frac = bits & 0xFFFFFFFFFFFFFull;

    if(exp == 0) {
        return frac == 0 ? FP_ZERO : FP_SUBNORMAL;
    }

    if(exp == 0x7FF) {
        return frac == 0 ? FP_INFINITE : FP_NAN;
    }

    return FP_NORMAL;
}
