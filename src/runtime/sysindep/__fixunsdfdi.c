#include "runtime.h"
#include <limits.h>
#include <string.h>

unsigned long long __fixunsdfdi(double a) {
    if(a != a || a <= 0.0)
        return 0; // NaN or negative

    unsigned long long bits;
    memcpy(&bits, &a, sizeof(bits));

    int exponent_raw = (bits >> 52) & 0x7FF;
    unsigned long long mantissa_raw = bits & 0xFFFFFFFFFFFFFULL;

    if(exponent_raw == 0x7FF)
        return ULLONG_MAX; // Infinity

    int exponent = exponent_raw - 1023;
    if(exponent < 0)
        return 0;
    if(exponent >= 64)
        return ULLONG_MAX;

    unsigned long long result;
    if(exponent_raw == 0) { // Subnormal number
        result = mantissa_raw;
    } else { // Normal number
        result = mantissa_raw | (1ULL << 52);
    }

    if(exponent > 52)
        return result << (exponent - 52);
    else
        return result >> (52 - exponent);
}
