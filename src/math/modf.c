#include <math.h>
#include <stdint.h>

double modf(double value, double *iptr) {
    union { double d; uint64_t u; } u;
    uint64_t mask;
    int exponent;

    u.d = value;
    exponent = (int)((u.u >> 52) & 0x7FF) - 1023;

    if (exponent < 0) {
        *iptr = (value < 0.0) ? -0.0 : 0.0;
        return value;
    } else if (exponent >= 52) {
        *iptr = value;
        return (value < 0.0) ? -0.0 : 0.0; /* Handle NaN/Inf implicity via bits */
    }

    mask = -1ULL >> (12 + exponent); 
    if ((u.u & mask) == 0) {
        *iptr = value;
        return (value < 0.0) ? -0.0 : 0.0;
    }

    u.u &= ~mask;
    *iptr = u.d;
    return value - u.d;
}
