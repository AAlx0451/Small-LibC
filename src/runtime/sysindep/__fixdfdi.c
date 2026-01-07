#include <limits.h>
#include <string.h>

long long __fixdfdi(double a) {
    if (a != a) return 0; // NaN

    unsigned long long bits;
    memcpy(&bits, &a, sizeof(bits));

    int sign = (bits >> 63) ? -1 : 1;
    int exponent_raw = (bits >> 52) & 0x7FF;
    unsigned long long mantissa_raw = bits & 0xFFFFFFFFFFFFFULL;

    if (exponent_raw == 0x7FF) { // Infinity
        return sign > 0 ? LLONG_MAX : LLONG_MIN;
    }

    int exponent = exponent_raw - 1023;
    if (exponent < 0) return 0;
    if (exponent >= 63) { // Out of range for long long
        return sign > 0 ? LLONG_MAX : LLONG_MIN;
    }

    unsigned long long result;
    if (exponent_raw == 0) { // Subnormal number
        result = mantissa_raw;
    } else { // Normal number
        result = mantissa_raw | (1ULL << 52);
    }
    
    if (exponent > 52) 
        result <<= (exponent - 52);
    else 
        result >>= (52 - exponent);

    if (sign < 0 && result > (1ULL << 63)) return LLONG_MIN; // Overflow to LLONG_MIN
    if (sign < 0 && result == (1ULL << 63)) return LLONG_MIN; // Avoid UB on negation

    return (sign < 0) ? -(long long)result : (long long)result;
}
