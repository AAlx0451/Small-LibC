#include <limits.h>

unsigned long long __fixunsdfdi(double a) {
    union {
        double d;
        unsigned long long u;
    } conv;
    conv.d = a;
    unsigned long long bits = conv.u;
    const unsigned long long sign = (bits >> 63);
    const int biased_exponent = (bits >> 52) & 0x7FF; // 11 бит экспоненты
    const unsigned long long mantissa = bits & 0x000FFFFFFFFFFFFFULL;
    if (sign == 1) {
        return 0;
    }
    if (biased_exponent == 0x7FF) {
        return ULLONG_MAX;
    }
    if (biased_exponent == 0) {
        return 0;
    }

    const int exponent = biased_exponent - 1023;
    if (exponent < 0) {
        return 0;
    }

    if (exponent >= (sizeof(unsigned long long) * CHAR_BIT)) {
        return ULLONG_MAX;
    }

    const unsigned long long full_mantissa = (1ULL << 52) | mantissa;
    if (exponent < 52) {
        return full_mantissa >> (52 - exponent);
    } else {
        return full_mantissa << (exponent - 52);
    }
}
