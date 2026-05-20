/* This file has been put into the public domain by its author. */

#include <errno.h>
#include <fenv.h>
#include <math.h>

/* Sollya coeffs: max error < 2^-63 */
static const double SINH_C3 = 0x1.5555555555555p-3;
static const double SINH_C5 = 0x1.11111111111b8p-7;
static const double SINH_C7 = 0x1.a01a019ffb84ap-13;
static const double SINH_C9 = 0x1.71de3a8320a16p-19;
static const double SINH_C11 = 0x1.ae6406c09b216p-26;
static const double SINH_C13 = 0x1.617a093ed6592p-33;
static const double SINH_C15 = 0x1.3b978be167b3fp-41;
static const double SINH_C17 = 0x1.7d8fbef1761d7p-43;
static const double SINH_C19 = -0x1.5513ce8bc09b1p-44;
static const double SINH_C21 = 0x1.08ca040a29f95p-46;

double sinh(double x)
{
    double abs_x = fabs(x);

    /* special cases handling */

    /* If x is NaN, a NaN shall be returned. */
    if (isnan(x)) {
        return x;
    }

    /* If x is +/-Inf, x shall be returned. */
    if (isinf(x)) {
        return x;
    }

    /* If x is +/-0, x shall be returned. */
    if (x == 0x0.0p+0) {
        return x;
    }

    /* Range Error: The value x is subnormal. */
    if (fpclassify(x) == FP_SUBNORMAL) {
        if (math_errhandling & MATH_ERRNO)
            errno = ERANGE;
        if (math_errhandling & MATH_ERREXCEPT)
            feraiseexcept(FE_UNDERFLOW);
        return x;
    }

    double x2, p, E, abs_res;

    if (abs_x < 0x1.0p+0) {      /* 1.0 */
        if (abs_x < 0x1.0p-28) { /* Guards against underflow in x^3 * p(x^2) */
            abs_res = abs_x;
        } else {
            x2 = abs_x * abs_x;

            p = SINH_C21;
            p = fma(p, x2, SINH_C19);
            p = fma(p, x2, SINH_C17);
            p = fma(p, x2, SINH_C15);
            p = fma(p, x2, SINH_C13);
            p = fma(p, x2, SINH_C11);
            p = fma(p, x2, SINH_C9);
            p = fma(p, x2, SINH_C7);
            p = fma(p, x2, SINH_C5);
            p = fma(p, x2, SINH_C3);

            /* sinh(|x|) = |x| + |x|^3 * p(|x|^2) */
            abs_res = fma(abs_x * x2, p, abs_x);
        }
    } else if (abs_x < 0x1.6p+4) { /* 22.0 */
        E = exp(abs_x);
        /* fma minimizes rounding errors: (E/2) - (1/2E) */
        abs_res = fma(E, 0x1.0p-1, -0x1.0p-1 / E);

    } else if (abs_x < 0x1.62e42fefa39efp+9) { /* ~709.782712893384 */
        /*
         * For x >= 22, the 1/E term is strictly smaller than 0.5 ULP.
         * x < ~709.78 is the threshold where standard exp() overflows.
         */
        E = exp(abs_x);
        abs_res = E * 0x1.0p-1;

    } else if (abs_x < 0x1.633ce8fb9f87dp+9) { /* ~710.4758600739439 */
        /* exp(x) overflows, but sinh(x) does not. */
        E = exp(abs_x * 0x1.0p-1);
        abs_res = (E * 0x1.0p-1) * E;

    } else {
        /* Range Error: The result would cause an overflow. */
        if (math_errhandling & MATH_ERRNO)
            errno = ERANGE;
        if (math_errhandling & MATH_ERREXCEPT)
            feraiseexcept(FE_OVERFLOW);
        abs_res = HUGE_VAL;
    }

    /* Restore original sign */
    return (x < 0x0.0p+0) ? -abs_res : abs_res;
}
