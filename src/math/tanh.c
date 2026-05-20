/* This file has been put into the public domain by its author. */

#include <math.h>
#include <errno.h>
#include <fenv.h>

/* Sollya generated coefficients for tanh */
/* Infinity norm absolute error: 0x1.7cc5a2f54c12e8a05c8b5765782fcfdd5a06888a56a971503p-58 */
static const double POLY_C3_HI  = -0x1.555555555548p-2;
static const double POLY_C3_LO  = -0x1.15d295316f22bp-56;
static const double POLY_C5_HI  =  0x1.1111111108b7dp-3;
static const double POLY_C5_LO  =  0x1.78d8aaf20a567p-57;
static const double POLY_C7_HI  = -0x1.ba1ba1b61dfa4p-5;
static const double POLY_C7_LO  = -0x1.5242eed767168p-60;
static const double POLY_C9_HI  =  0x1.664f4800bdf5p-6;
static const double POLY_C9_LO  = -0x1.184316eaec7b5p-60;
static const double POLY_C11    = -0x1.226e2b2f0f58ap-7;
static const double POLY_C13    =  0x1.d6d2c0283ee52p-9;
static const double POLY_C15    = -0x1.7d9960cf7e43cp-10;
static const double POLY_C17    =  0x1.3513dd54b8924p-11;
static const double POLY_C19    = -0x1.f2cb9a38581a2p-13;
static const double POLY_C21    =  0x1.8c46118d7fd5bp-14;
static const double POLY_C23    = -0x1.2c324d64dc5bp-15;
static const double POLY_C25    =  0x1.96c86bd8cccc7p-17;
static const double POLY_C27    = -0x1.bcdab827b1f37p-19;
static const double POLY_C29    =  0x1.4f2155c6440cp-21;
static const double POLY_C31    = -0x1.f8cc4f87e5b66p-25;

double tanh(double x) {
    double abs_x = fabs(x);

    /* special cases handling */

    /* If x is NaN, a NaN shall be returned. */
    if (isnan(x)) {
        return x;
    }

    /* If x is +/-0, x shall be returned. */
    if (x == 0x0.0p+0) {
        return x;
    }

    /* If x is +/-Inf, +/-1 shall be returned. */
    if (isinf(x)) {
        return (x < 0x0.0p+0) ? -0x1.0p+0 : 0x1.0p+0;
    }

    /* Range Error: The value of x is subnormal. */
    if (fpclassify(x) == FP_SUBNORMAL) {
        if (math_errhandling & MATH_ERRNO) errno = ERANGE;
        if (math_errhandling & MATH_ERREXCEPT) feraiseexcept(FE_UNDERFLOW);
        return x;
    }

    if (abs_x >= 0x1.6p+4) { /* 22.0 */
        /* At large x, tanh(x) converges to +/- 1.0. */
        return (x < 0x0.0p+0) ? -0x1.0p+0 : 0x1.0p+0;
        
    } else if (abs_x >= 0x1.0p+0) { /* 1.0 */
        /* Interval [1.0, 22.0): use the identity 1 - 2 / (exp(2x) + 1).
           The subtraction compresses the relative error from exp(). */
        double e2x = exp(0x1.0p+1 * abs_x);
        double res = 0x1.0p+0 - 0x1.0p+1 / (e2x + 0x1.0p+0);
        return (x < 0x0.0p+0) ? -res : res;
        
    } else if (abs_x < 0x1.0p-28) { /* 2^-28 */
        /* Exact handling for very small numbers. */
        return x; 
    }

    /* Interval [2^-28, 1.0): Minimax polynomial evaluation. */
    double t = abs_x * abs_x;
    double t_lo = fma(abs_x, abs_x, -t);
    
    double H, P_lo, P_hi, P_hi_final, sum, y, res;

    /* Evaluate the double-precision tail using standard Horner */
    H = POLY_C31;
    H = fma(t, H, POLY_C29);
    H = fma(t, H, POLY_C27);
    H = fma(t, H, POLY_C25);
    H = fma(t, H, POLY_C23);
    H = fma(t, H, POLY_C21);
    H = fma(t, H, POLY_C19);
    H = fma(t, H, POLY_C17);
    H = fma(t, H, POLY_C15);
    H = fma(t, H, POLY_C13);
    H = fma(t, H, POLY_C11);

    /* Evaluate double-double parts */
    P_lo = fma(t, H, POLY_C9_LO);
    P_hi = POLY_C9_HI;

    P_lo = fma(t, P_lo, POLY_C7_LO);
    P_hi = fma(t, P_hi, POLY_C7_HI);

    P_lo = fma(t, P_lo, POLY_C5_LO);
    P_hi = fma(t, P_hi, POLY_C5_HI);

    P_lo = fma(t, P_lo, POLY_C3_LO);

    /* Final assembly compensating for the x^2 rounding error (t_lo) */
    P_hi_final = fma(t, P_hi, POLY_C3_HI);
    sum = fma(t_lo, P_hi_final, t * P_lo);
    y = fma(t, P_hi_final, sum);

    /* Final fma: x + x * (t * P(t)) */
    res = fma(abs_x, y, abs_x);
    return (x < 0x0.0p+0) ? -res : res;
}
