#include <limits.h>

long long __fixdfdi(double a) {
    if (a == 0.0) {
        return 0;
    }

    union {
        double d;
        unsigned long long u;
    } conv;
    conv.d = a;
    unsigned long long bits = conv.u;
    unsigned long long sign_bit = (bits >> 63) & 1;
    int exponent = (bits >> 52) & 0x7FF;
    unsigned long long mantissa = bits & 0x000FFFFFFFFFFFFFULL;
    int shift = exponent - 1023;
    if (shift < 0) {
        return 0;
    }

    mantissa |= (1ULL << 52);
    unsigned long long result = 0;
    if (shift > 52) {
        if (shift >= 63) {
            if (sign_bit && shift == 63 && mantissa == (1ULL << 52)) {
                return LLONG_MIN;
            }
            return sign_bit ? LLONG_MIN : LLONG_MAX;
        }
        result = mantissa << (shift - 52);
    } else {
        result = mantissa >> (52 - shift);
    }

    if (sign_bit) {
        return -(long long)result;
    } else {
        return (long long)result;
    }
}
