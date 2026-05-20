/* This file has been put into the public domain by its author. */

#include <errno.h>
#include <fenv.h>
#include <float.h>
#include <math.h>

/* Sollya generated coefficients for pow log2(1+x) */
/* Infinity norm absolute error: 0x1.b9dbcf8da85a2fb06b107956c46b0408f091a67b9f48dc1a78p-70 */
static const double LOG2_C1_HI = 0x1.71547652b82fep0;
static const double LOG2_C1_LO = 0x1.78c779784a708p-56;
static const double LOG2_C2_HI = -0x1.71547652b82fep-1;
static const double LOG2_C2_LO = -0x1.b07477bc26ea4p-57;
static const double LOG2_C3_HI = 0x1.ec709dc3a03fcp-2;
static const double LOG2_C3_LO = 0x1.3adb8102dcc2ep-57;
static const double LOG2_C4_HI = -0x1.71547652b82eep-2;
static const double LOG2_C4_LO = -0x1.d740fdfebbc9p-60;
static const double LOG2_C5 = 0x1.2776c50ef9d69p-2;
static const double LOG2_C6 = -0x1.ec709dc3a1e72p-3;
static const double LOG2_C7 = 0x1.a61762a7982acp-3;
static const double LOG2_C8 = -0x1.7154765204909p-3;
static const double LOG2_C9 = 0x1.484b13dd4f1e4p-3;
static const double LOG2_C10 = -0x1.2776c53c212c1p-3;
static const double LOG2_C11 = 0x1.0c9a83c95604fp-3;
static const double LOG2_C12 = -0x1.ec708f61340a7p-4;
static const double LOG2_C13 = 0x1.c68f78a3f6a05p-4;
static const double LOG2_C14 = -0x1.a618e17b97d58p-4;
static const double LOG2_C15 = 0x1.89f281afe9c3cp-4;
static const double LOG2_C16 = -0x1.713a74f742bf5p-4;
static const double LOG2_C17 = 0x1.5b90ca3be17bfp-4;
static const double LOG2_C18 = -0x1.4968e5b1418ebp-4;
static const double LOG2_C19 = 0x1.388987c7c1f2dp-4;
static const double LOG2_C20 = -0x1.2079e021989dp-4;
static const double LOG2_C21 = 0x1.087482772d29bp-4;
static const double LOG2_C22 = -0x1.1d007df845eb2p-4;
static const double LOG2_C23 = 0x1.55129196c2d0cp-4;
static const double LOG2_C24 = -0x1.268cee314425dp-4;
static const double LOG2_C25 = 0x1.cc613a18a3551p-6;

/* Sollya generated coefficients for pow exp2(x) - 1 */
/* Infinity norm absolute error: 0x1.1cf47a64c3d7f4e7d0bc085dbdbce1264c1c1907d152655732p-69 */
static const double EXP2_C1_HI = 0x1.62e42fefa39efp-1;
static const double EXP2_C1_LO = 0x1.ac5c600cb0326p-56;
static const double EXP2_C2_HI = 0x1.ebfbdff82c58fp-3;
static const double EXP2_C2_LO = -0x1.62e8e00b21879p-57;
static const double EXP2_C3_HI = 0x1.c6b08d704a0bfp-5;
static const double EXP2_C3_LO = 0x1.8c236f1a82f58p-60;
static const double EXP2_C4 = 0x1.3b2ab6fba4e7dp-7;
static const double EXP2_C5 = 0x1.5d87fe78a6856p-10;
static const double EXP2_C6 = 0x1.430912f86a101p-13;
static const double EXP2_C7 = 0x1.ffcbfc581cbd7p-17;
static const double EXP2_C8 = 0x1.62c02248e06f6p-20;
static const double EXP2_C9 = 0x1.b5253e8b8f6edp-24;
static const double EXP2_C10 = 0x1.e4cef93466047p-28;
static const double EXP2_C11 = 0x1.e8c6ba7066118p-32;
static const double EXP2_C12 = 0x1.c4c4060e82efcp-36;
static const double EXP2_C13 = 0x1.868a87ec18839p-40;

/* Error-free transformation for addition (Double-Double) */
static inline void two_sum(double a, double b, double *s, double *e)
{
    double v;
    *s = a + b;
    v = *s - a;
    *e = (a - (*s - v)) + (b - v);
}

/* Error-free transformation for multiplication using FMA */
static inline void two_prod(double a, double b, double *p, double *e)
{
    *p = a * b;
    *e = fma(a, b, -(*p));
}

double pow(double x, double y)
{
    int E_int;
    int is_res_neg = 0;
    double M, m, E_d;
    double H, He, Ahi, Alo, Phi, Plo, Shi, Slo;
    double Lhi, Llo, final_Lhi, final_Llo;
    double Whi, Wlo, W_hi, W_lo;
    double Kd, rhi, rlo, rh, rl;
    double E_hi, E_lo, final_mantissa, res;

    /* Special Cases Processing */

    if (y == 0x0.0p+0)
        return 0x1.0p+0;
    if (x == 0x1.0p+0)
        return 0x1.0p+0;

    /* NaN propagation */
    if (isnan(x) || isnan(y))
        return x + y;

    if (isinf(y)) {
        if (x <= -0x1.0p+0 && x >= -0x1.0p+0)
            return 0x1.0p+0; /* x == -1.0 */
        if (fabs(x) < 0x1.0p+0)
            return (y < 0x0.0p+0) ? HUGE_VAL : 0x0.0p+0;
        if (fabs(x) > 0x1.0p+0)
            return (y < 0x0.0p+0) ? 0x0.0p+0 : HUGE_VAL;
    }

    if (isinf(x)) {
        if (x > 0x0.0p+0) {
            return (y > 0x0.0p+0) ? HUGE_VAL : 0x0.0p+0;
        } else {
            int y_i = 0;
            double abs_y = fabs(y);
            if (abs_y >= 0x1.0p+52) { /* 2^52 */
                y_i = 2;
            } else if (floor(abs_y) == abs_y) {
                double half = abs_y * 0x1.0p-1;
                if (floor(half) == half) {
                    y_i = 2;
                } else {
                    y_i = 1;
                }
            }

            if (y > 0x0.0p+0)
                return (y_i == 1) ? -HUGE_VAL : HUGE_VAL;
            if (y < 0x0.0p+0)
                return (y_i == 1) ? -0x0.0p+0 : 0x0.0p+0;
        }
    }

    if (x == 0x0.0p+0) {
        int y_i = 0;
        double abs_y = fabs(y);
        if (abs_y >= 0x1.0p+52) {
            y_i = 2;
        } else if (floor(abs_y) == abs_y) {
            double half = abs_y * 0x1.0p-1;
            if (floor(half) == half) {
                y_i = 2;
            } else {
                y_i = 1;
            }
        }

        int is_neg_zero = (fpclassify(x) == FP_ZERO && signbit(x));
        if (y > 0x0.0p+0) {
            return (is_neg_zero && y_i == 1) ? -0x0.0p+0 : 0x0.0p+0;
        } else {
            /* Pole Error */
            if (math_errhandling & MATH_ERRNO)
                errno = ERANGE;
            if (math_errhandling & MATH_ERREXCEPT)
                feraiseexcept(FE_DIVBYZERO);

            return (is_neg_zero && y_i == 1) ? -HUGE_VAL : HUGE_VAL;
        }
    }

    if (x < 0x0.0p+0) {
        int y_i = 0;
        double abs_y = fabs(y);
        if (abs_y >= 0x1.0p+52) {
            y_i = 2;
        } else if (floor(abs_y) == abs_y) {
            double half = abs_y * 0x1.0p-1;
            if (floor(half) == half) {
                y_i = 2;
            } else {
                y_i = 1;
            }
        }

        if (y_i == 0) {
            /* Domain Error */
            if (math_errhandling & MATH_ERRNO)
                errno = EDOM;
            if (math_errhandling & MATH_ERREXCEPT)
                feraiseexcept(FE_INVALID);

            return nan("");
        }
        if (y_i == 1)
            is_res_neg = 1;
        x = -x; /* Make positive for the core evaluation */
    }

    /* Precise log2(x) */
    M = frexp(x, &E_int);
    if (M < 0x1.6a09e667f3bcdp-1) { /* sqrt(2)/2 */
        M *= 0x1.0p+1;
        E_int -= 1;
    }
    m = M - 0x1.0p+0; /* Exact by Sterbenz Lemma */
    E_d = (double)E_int;

    /* Evaluate standard Double precision tail of log2 */
    H = LOG2_C25;
    H = fma(m, H, LOG2_C24);
    H = fma(m, H, LOG2_C23);
    H = fma(m, H, LOG2_C22);
    H = fma(m, H, LOG2_C21);
    H = fma(m, H, LOG2_C20);
    H = fma(m, H, LOG2_C19);
    H = fma(m, H, LOG2_C18);
    H = fma(m, H, LOG2_C17);
    H = fma(m, H, LOG2_C16);
    H = fma(m, H, LOG2_C15);
    H = fma(m, H, LOG2_C14);
    H = fma(m, H, LOG2_C13);
    H = fma(m, H, LOG2_C12);
    H = fma(m, H, LOG2_C11);
    H = fma(m, H, LOG2_C10);
    H = fma(m, H, LOG2_C9);
    H = fma(m, H, LOG2_C8);
    H = fma(m, H, LOG2_C7);
    H = fma(m, H, LOG2_C6);
    H = fma(m, H, LOG2_C5);

    /* Double-Double Horner evaluation for log2 */
    Ahi = LOG2_C4_HI;
    Alo = LOG2_C4_LO + m * H;

    two_prod(Ahi, m, &Phi, &Plo);
    Plo += Alo * m;
    two_sum(Phi, LOG2_C3_HI, &Shi, &Slo);
    Slo += Plo + LOG2_C3_LO;
    Ahi = Shi + Slo;
    Alo = (Shi - Ahi) + Slo;

    two_prod(Ahi, m, &Phi, &Plo);
    Plo += Alo * m;
    two_sum(Phi, LOG2_C2_HI, &Shi, &Slo);
    Slo += Plo + LOG2_C2_LO;
    Ahi = Shi + Slo;
    Alo = (Shi - Ahi) + Slo;

    two_prod(Ahi, m, &Phi, &Plo);
    Plo += Alo * m;
    two_sum(Phi, LOG2_C1_HI, &Shi, &Slo);
    Slo += Plo + LOG2_C1_LO;
    Ahi = Shi + Slo;
    Alo = (Shi - Ahi) + Slo;

    /* Multiply by m at the end */
    two_prod(Ahi, m, &Phi, &Plo);
    Plo += Alo * m;

    /* Combine E + log2(1+m) completely accurately */
    two_sum(E_d, Phi, &Lhi, &Llo);
    Llo += Plo;
    final_Lhi = Lhi + Llo;
    final_Llo = (Lhi - final_Lhi) + Llo;

    /* Precise Multiplication W = y * log2(x) */
    two_prod(y, final_Lhi, &Whi, &Wlo);
    Wlo += y * final_Llo;
    W_hi = Whi + Wlo;
    W_lo = (Whi - W_hi) + Wlo;

    /* Pre-check extreme ranges to prevent massive intermediate integer overflow */
    if (W_hi >= 0x1.0p+10) { /* 1024.0 */
        if (math_errhandling & MATH_ERRNO)
            errno = ERANGE;
        if (math_errhandling & MATH_ERREXCEPT)
            feraiseexcept(FE_OVERFLOW);

        return is_res_neg ? -HUGE_VAL : HUGE_VAL;
    }
    if (W_hi <= -0x1.0c8p+10) { /* -1075.0 */
        if (math_errhandling & MATH_ERRNO)
            errno = ERANGE;
        if (math_errhandling & MATH_ERREXCEPT)
            feraiseexcept(FE_UNDERFLOW);

        return is_res_neg ? -0x0.0p+0 : 0x0.0p+0;
    }

    /* Precise exp2(W) */
    Kd = floor(W_hi + 0x1.0p-1);
    rhi = W_hi - Kd; /* Exact Sterbenz subtraction */
    rlo = W_lo;

    rh = rhi;
    rl = rlo;

    /* Evaluate standard Double precision tail of exp2 */
    He = EXP2_C13;
    He = fma(rh, He, EXP2_C12);
    He = fma(rh, He, EXP2_C11);
    He = fma(rh, He, EXP2_C10);
    He = fma(rh, He, EXP2_C9);
    He = fma(rh, He, EXP2_C8);
    He = fma(rh, He, EXP2_C7);
    He = fma(rh, He, EXP2_C6);
    He = fma(rh, He, EXP2_C5);
    He = fma(rh, He, EXP2_C4);

    /* Double-Double Horner evaluation for exp2 */
    Ahi = EXP2_C3_HI;
    Alo = EXP2_C3_LO + rh * He;

    /* Multiplications carefully expand DD * DD accurately enough for exp2 tail */
    two_prod(rh, Ahi, &Phi, &Plo);
    Plo += rh * Alo + rl * Ahi;
    two_sum(Phi, EXP2_C2_HI, &Shi, &Slo);
    Slo += Plo + EXP2_C2_LO;
    Ahi = Shi + Slo;
    Alo = (Shi - Ahi) + Slo;

    two_prod(rh, Ahi, &Phi, &Plo);
    Plo += rh * Alo + rl * Ahi;
    two_sum(Phi, EXP2_C1_HI, &Shi, &Slo);
    Slo += Plo + EXP2_C1_LO;
    Ahi = Shi + Slo;
    Alo = (Shi - Ahi) + Slo;

    two_prod(rh, Ahi, &Phi, &Plo);
    Plo += rh * Alo + rl * Ahi;
    E_hi = Phi + Plo;
    E_lo = (Phi - E_hi) + Plo;

    /* Assemble Final Value: 2^K * (1 + E) */
    two_sum(0x1.0p+0, E_hi, &Shi, &Slo);
    Slo += E_lo;
    final_mantissa = Shi + Slo;

    res = ldexp(final_mantissa, (int)Kd);

    /* Exact final bound verification for strict boundary underflows/overflows */
    if (isinf(res)) {
        if (math_errhandling & MATH_ERRNO)
            errno = ERANGE;
        if (math_errhandling & MATH_ERREXCEPT)
            feraiseexcept(FE_OVERFLOW);
    }

    /* Caught an unrepresentable number or a subnormal (which also warrants FE_UNDERFLOW) */
    if (!isinf(res) && fabs(res) < DBL_MIN) {
        if (math_errhandling & MATH_ERRNO)
            errno = ERANGE;
        if (math_errhandling & MATH_ERREXCEPT)
            feraiseexcept(FE_UNDERFLOW);
    }

    return is_res_neg ? -res : res;
}
