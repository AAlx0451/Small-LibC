/* This file has been put into the public domain by its author. */

#include <math.h>
#include <errno.h>
#include <fenv.h>

/* Sollya generated coefficients for log10(1+x) */
/* Infinity norm absolute error: 0x1.d191fcc6e89edda1f43dfc2be82e2a7fbdcb8f7b3c5971651cp-64 */
static const double LOG10_C1_HI  =  0x1.bcb7b1526e50ep-2;
static const double LOG10_C1_LO  =  0x1.e06dde6bedd68p-58;
static const double LOG10_C2_HI  = -0x1.bcb7b1526e511p-3;
static const double LOG10_C2_LO  =  0x1.f6ee01665260ep-58;
static const double LOG10_C3_HI  =  0x1.287a7636f43e2p-3;
static const double LOG10_C3_LO  =  0x1.3feb7a64ad6b1p-57;
static const double LOG10_C4_HI  = -0x1.bcb7b1526dd4p-4;
static const double LOG10_C4_LO  = -0x1.1baef7fd0eecp-58;
static const double LOG10_C5     =  0x1.63c62775164cfp-4;
static const double LOG10_C6     = -0x1.287a76373142dp-4;
static const double LOG10_C7     =  0x1.fc3fa62127139p-5;
static const double LOG10_C8     = -0x1.bcb7b13960798p-5;
static const double LOG10_C9     =  0x1.8b4df03a5badcp-5;
static const double LOG10_C10    = -0x1.63c6292ef7f42p-5;
static const double LOG10_C11    =  0x1.436eb08d7a48dp-5;
static const double LOG10_C12    = -0x1.287aa1a922cbep-5;
static const double LOG10_C13    =  0x1.11a449d3cf3c5p-5;
static const double LOG10_C14    = -0x1.fc2a331f834fep-6;
static const double LOG10_C15    =  0x1.db26eb9e67cd7p-6;
static const double LOG10_C16    = -0x1.be7bd7a0a0a31p-6;
static const double LOG10_C17    =  0x1.9d15580e51453p-6;
static const double LOG10_C18    = -0x1.787206851296dp-6;
static const double LOG10_C19    =  0x1.823fde2d72cb1p-6;
static const double LOG10_C20    = -0x1.c5d26a232b31fp-6;
static const double LOG10_C21    =  0x1.9aa39580b2de4p-6;
static const double LOG10_C22    = -0x1.58b24b13dc944p-7;

/* Precise log10(2) */
static const double LOG10_2_HI   =  0x1.34413509f79ffp-2;
static const double LOG10_2_LO   = -0x1.9dc1da994fd21p-59;

/* Error-free transformation for addition */
static inline void two_sum(double a, double b, double *s, double *e) {
    double v;
    *s = a + b;
    v = *s - a;
    *e = (a - (*s - v)) + (b - v);
}

/* Error-free transformation for multiplication using FMA */
static inline void two_prod(double a, double b, double *p, double *e) {
    *p = a * b;
    *e = fma(a, b, -(*p));
}

double log10(double x) {
    /* special cases handling */

    /* If x is NaN, a NaN shall be returned. */
    if (isnan(x)) {
        return x;
    }

    /* If x is 1, +0 shall be returned. */
    if (x == 0x1.0p+0) {
        return 0x0.0p+0;
    }

    /* If x is +Inf, +Inf shall be returned. */
    if (isinf(x) && x > 0x0.0p+0) {
        return x;
    }

    /* Pole Error: The value of x is zero. */
    if (x == 0x0.0p+0) {
        if (math_errhandling & MATH_ERRNO) errno = ERANGE;
        if (math_errhandling & MATH_ERREXCEPT) feraiseexcept(FE_DIVBYZERO);
        return -HUGE_VAL;
    }

    /* Domain Error: The finite value of x is negative, or x is -Inf. */
    if (x < 0x0.0p+0) {
        if (math_errhandling & MATH_ERRNO) errno = EDOM;
        if (math_errhandling & MATH_ERREXCEPT) feraiseexcept(FE_INVALID);
        return nan("");
    }

    int E_int, subnormal_shift;
    double M, m, E_d;
    double H, Ahi, Alo, Phi, Plo, Shi, Slo;
    double P_hi, P_lo;
    double tmp_hi, tmp_lo, E_log2_hi, E_log2_lo;
    double R_hi, R_lo, tail_sum;

    subnormal_shift = 0;
    if (x < DBL_MIN) {
        x *= 0x1.0p+53;    /* 9007199254740992.0 */
        subnormal_shift = 53;
    }

    M = frexp(x, &E_int);
    E_int -= subnormal_shift;

    if (M < 0x1.6a09e667f3bcdp-1) { /* 0.70710678118654752440 (sqrt(2)/2) */
        M *= 0x1.0p+1;
        E_int -= 1;
    }
    
    m = M - 0x1.0p+0;
    E_d = (double)E_int;

    /* Tail polynomial evaluation (C22 down to C5) */
    H = LOG10_C22;
    H = fma(m, H, LOG10_C21);
    H = fma(m, H, LOG10_C20);
    H = fma(m, H, LOG10_C19);
    H = fma(m, H, LOG10_C18);
    H = fma(m, H, LOG10_C17);
    H = fma(m, H, LOG10_C16);
    H = fma(m, H, LOG10_C15);
    H = fma(m, H, LOG10_C14);
    H = fma(m, H, LOG10_C13);
    H = fma(m, H, LOG10_C12);
    H = fma(m, H, LOG10_C11);
    H = fma(m, H, LOG10_C10);
    H = fma(m, H, LOG10_C9);
    H = fma(m, H, LOG10_C8);
    H = fma(m, H, LOG10_C7);
    H = fma(m, H, LOG10_C6);
    H = fma(m, H, LOG10_C5);

    /* Double-Double Horner evaluation for the first 4 coefficients */
    Ahi = LOG10_C4_HI;
    Alo = LOG10_C4_LO + m * H;

    two_prod(Ahi, m, &Phi, &Plo); Plo += Alo * m;
    two_sum(Phi, LOG10_C3_HI, &Shi, &Slo); Slo += Plo + LOG10_C3_LO;
    Ahi = Shi + Slo; Alo = (Shi - Ahi) + Slo;

    two_prod(Ahi, m, &Phi, &Plo); Plo += Alo * m;
    two_sum(Phi, LOG10_C2_HI, &Shi, &Slo); Slo += Plo + LOG10_C2_LO;
    Ahi = Shi + Slo; Alo = (Shi - Ahi) + Slo;

    two_prod(Ahi, m, &Phi, &Plo); Plo += Alo * m;
    two_sum(Phi, LOG10_C1_HI, &Shi, &Slo); Slo += Plo + LOG10_C1_LO;
    Ahi = Shi + Slo; Alo = (Shi - Ahi) + Slo;

    /* P(m) = m * A */
    two_prod(Ahi, m, &P_hi, &P_lo);
    P_lo += Alo * m;

    /* Exponent multiplication */
    two_prod(E_d, LOG10_2_HI, &tmp_hi, &tmp_lo);
    E_log2_lo = fma(E_d, LOG10_2_LO, tmp_lo);
    E_log2_hi = tmp_hi;

    /* Final accurate summation */
    two_sum(E_log2_hi, P_hi, &R_hi, &R_lo);
    tail_sum = R_lo + E_log2_lo + P_lo;

    return R_hi + tail_sum;
}
