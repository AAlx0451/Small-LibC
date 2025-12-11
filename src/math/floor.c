#include <math.h>
#include <stdint.h>

double floor(double x) {
    union {
        double f;
        uint64_t i;
    } u; 
    int exponent;
    uint64_t fractional_mask;
    u.f = x;
    exponent = (int)((u.i >> 52) & 0x7FF) - 1023;
    if (exponent >= 52) {
        return x;
    }

    if (exponent < 0) {
        if ((u.i >> 63) == 0) {
            return 0.0; 
        }
        if (x == -0.0) {
             return -0.0;
        }
        return -1.0;
    }

    fractional_mask = 0xFFFFFFFFFFFFFULL >> exponent;
    if ((u.i & fractional_mask) == 0) {
        return x;
    }

    if ((u.i >> 63) == 0) {
        u.i &= ~fractional_mask;
    } else {
        u.i &= ~fractional_mask;
        u.f -= 1.0;
    }

    return u.f;
}
