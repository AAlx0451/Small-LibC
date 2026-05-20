/* This file has been put into the public domain by its author. */

#include <math.h>
#include <errno.h>
#include <fenv.h>

/* Sollya generated coefficients for cosh on interval [2^-20, 2.0] */
/* Infinity norm absolute error: 0x1.0e9bff680d4d1d81bcd7e1092cf561986216aa35cd3a6964a4p-60 */
static const double POLY_C2_HI  =  0x1.0p-1;              /* Exactly 0.5 */
static const double POLY_C2_LO  =  0x1.0c64532f9b672p-55;
static const double POLY_C4_HI  =  0x1.555555555552bp-5;
static const double POLY_C4_LO  =  0x1.44f645b31ba49p-59;
static const double POLY_C6     =  0x1.6c16c16c17e35p-10;
static const double POLY_C8     =  0x1.a01a019f9f755p-16;
static const double POLY_C10    =  0x1.27e4fb94d5c4fp-22;
static const double POLY_C12    =  0x1.1eed868b1810ep-29;
static const double POLY_C14    =  0x1.939a4b7932e23p-37;
static const double POLY_C16    =  0x1.addaf1ade150fp-45;
static const double POLY_C18    =  0x1.7b787d60c3f16p-53;

double cosh(double x) {
    double abs_x = fabs(x);

    /* special cases handling */

    /* If x is NaN, a NaN shall be returned. */
    if (isnan(x)) {
        return x;
    }

    /* If x is +/-Inf, +Inf shall be returned. (No range error for exact Inf input) */
    if (isinf(x)) {
        return HUGE_VAL; 
    }

    /* If x is +/-0, the value 1.0 shall be returned. */
    if (x == 0x0.0p+0) {
        return 0x1.0p+0;
    }

    if (abs_x < 0x1.0p+1) { /* 2.0 */
        if (abs_x < 0x1.0p-28) { /* 2^-28 */
            /* cosh(0) is exactly 1.0. Guards against underflow. */
            return 0x1.0p+0;
        } 
        
        /* Minimax polynomial for the core [2^-28, 2.0) interval */
        double t = abs_x * abs_x;
        double t_lo = fma(abs_x, abs_x, -t);
        
        double H, P_lo, P_hi, P_hi_final, sum, y;

        /* Evaluate standard Double precision tail using Horner */
        H = POLY_C18;
        H = fma(t, H, POLY_C16);
        H = fma(t, H, POLY_C14);
        H = fma(t, H, POLY_C12);
        H = fma(t, H, POLY_C10);
        H = fma(t, H, POLY_C8);
        H = fma(t, H, POLY_C6);

        /* Fold in the Double-Double components */
        P_lo = fma(t, H, POLY_C4_LO);
        P_hi = POLY_C4_HI;

        P_lo = fma(t, P_lo, POLY_C2_LO);
        P_hi_final = fma(t, P_hi, POLY_C2_HI);

        /* Compensate for the x^2 rounding error (t_lo) */
        sum = fma(t_lo, POLY_C2_HI, t * P_lo);
        y = fma(t, P_hi_final, sum);

        /* 1.0 + y causes exactly one rounding step. */
        return 0x1.0p+0 + y;
        
    } else if (abs_x < 0x1.6p+4) { /* 22.0 */
        /* Direct evaluation: (e^x + 1/e^x) / 2 */
        double w = exp(abs_x);
        return fma(0x1.0p-1, w, 0x1.0p-1 / w);
        
    } else if (abs_x < 0x1.628p+9) { /* 709.0 */
        /* e^-x is mathematically invisible in Double precision. */
        return 0x1.0p-1 * exp(abs_x);
    } 

    /* abs_x >= 709.0 */
    /* Prevent early overflow inside exp(), but properly assemble the upper limit */
    double w = exp(abs_x * 0x1.0p-1);
    double res = (w * 0x1.0p-1) * w;

    /* Range Error: The result would cause an overflow. */
    if (isinf(res)) {
        if (math_errhandling & MATH_ERRNO) errno = ERANGE;
        if (math_errhandling & MATH_ERREXCEPT) feraiseexcept(FE_OVERFLOW);
    }

    return res;
}
