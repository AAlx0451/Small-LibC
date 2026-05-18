/* This file has been put into the public domain by its author. */

#include <math.h>
#include <errno.h>
#include <fenv.h>

double fmod(double x, double y) {
    int xchar, ychar;
    double t;

    int errx = fpclassify(x);
    int erry = fpclassify(y);

    if (errx == FP_NAN || erry == FP_NAN) {
        return (errx == FP_NAN) ? x : y;
    }

    if (errx == FP_INFINITE || erry == FP_ZERO) {
        if (math_errhandling & MATH_ERRNO) errno = EDOM;
        if (math_errhandling & MATH_ERREXCEPT) feraiseexcept(FE_INVALID);
        return nan(""); 
    }

    if (errx == FP_ZERO || erry == FP_INFINITE) {
        return x;
    }

    int neg = signbit(x);
    x = fabs(x);
    y = fabs(y);

    frexp(x, &xchar);
    frexp(y, &ychar);

    for (int n = xchar - ychar; 0 <= n; --n) {
        t = ldexp(y, n);
        if (t <= x) {
            x -= t;
        }
    }

    return neg ? -x : x;
}
