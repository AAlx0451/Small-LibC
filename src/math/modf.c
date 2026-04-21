/* This file has been put into the public domain by its author. */

#include <math.h>
#include <stdint.h>
#include <string.h>

double modf(double x, double *iptr) {
    uint64_t bits;
    memcpy(&bits, &x, 8);

    int exp = (int)((bits >> 52) & 0x7FF) - 1023;
    uint64_t sign_bit = bits & 0x8000000000000000ULL;

    if(exp < 0) {
        /* integer part is zero, fraction is x */
        uint64_t zero_bits = sign_bit;
        memcpy(iptr, &zero_bits, 8);
        return x;
    }

    if(exp >= 52) {
        /* x is integer or special case */
        *iptr = x;
        if(isnan(x))
            return x;
        uint64_t zero_bits = sign_bit;
        memcpy(&x, &zero_bits, 8);
        return x;
    }

    /* clear fractional bits */
    uint64_t mask = 0x000FFFFFFFFFFFFFULL >> exp;
    if((bits & mask) == 0) {
        *iptr = x;
        uint64_t zero_bits = sign_bit;
        memcpy(&x, &zero_bits, 8);
        return x;
    }

    uint64_t i_bits = bits & ~mask;
    memcpy(iptr, &i_bits, 8);
    return x - *iptr;
}
