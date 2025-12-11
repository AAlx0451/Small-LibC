#include <math.h>
#include <errno.h>
#include <stdint.h>

double sqrt(double x) {
    union {
        double f;
        uint64_t i;
    } u;

    /* Handle special cases: negative, NaN, Inf, -0.0 */
    u.f = x;
    if ((u.i >> 63)) {
        if ((u.i & 0x7FFFFFFFFFFFFFULL) == 0) return x; /* -0.0 -> -0.0 */
        errno = EDOM;
        return NAN;
    }
    if ((u.i & 0x7FF0000000000000ULL) == 0x7FF0000000000000ULL) {
        return x; /* Inf or NaN */
    }
    if (x == 0.0) return 0.0;

    /* Normalize subnormal numbers */
    int scale = 0;
    if ((u.i & 0x7FF0000000000000ULL) == 0) {
        x *= 4.503599627370496e15; /* 2^52 */
        u.f = x;
        scale = -52;
    }

    /* Initial approximation using bit manipulation */
    u.i = (u.i >> 1) + 0x1FF8000000000000ULL;
    double y = u.f;

    /* Heron's method (Newton-Raphson) iterations */
    /* 1st iteration */
    y = 0.5 * (y + x / y);
    /* 2nd iteration */
    y = 0.5 * (y + x / y);
    /* 3rd iteration (usually sufficient for double precision) */
    y = 0.5 * (y + x / y);

    /* Scale back if we adjusted subnormals */
    if (scale) {
        /* Multiplication by 2^(scale/2) */
        /* Since scale is -52, we need 2^-26 */
        y *= 1.4901161193847656e-8;
    }

    return y;
}
