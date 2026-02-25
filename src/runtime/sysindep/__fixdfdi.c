#include "runtime.h"
#include <limits.h>
#include <string.h>

long long __fixdfdi(double a) {
    unsigned long long bits, mantissa_raw, result;
    int sign, exponent_raw, exponent;
    if(a != a)
        return 0; // NaN

    memcpy(&bits, &a, sizeof(bits));

    sign = (bits >> 63) ? -1 : 1;
    exponent_raw = (bits >> 52) & 0x7FF;
    mantissa_raw = bits & 0xFFFFFFFFFFFFFULL;

    if(exponent_raw == 0x7FF) { // Infinity
        return sign > 0 ? LLONG_MAX : LLONG_MIN;
    }

    exponent = exponent_raw - 1023;
    if(exponent < 0)
        return 0;
    if(exponent >= 63) { // Out of range for long long
        return sign > 0 ? LLONG_MAX : LLONG_MIN;
    }

    if(exponent_raw == 0) { // Subnormal number
        result = mantissa_raw;
    } else { // Normal number
        result = mantissa_raw | (1ULL << 52);
    }

    if(exponent > 52)
        result <<= (exponent - 52);
    else
        result >>= (52 - exponent);

    if(sign < 0 && result > (1ULL << 63))
        return LLONG_MIN; // Overflow to LLONG_MIN
    if(sign < 0 && result == (1ULL << 63))
        return LLONG_MIN; // Avoid UB on negation

    return (sign < 0) ? -(long long)result : (long long)result;
}
