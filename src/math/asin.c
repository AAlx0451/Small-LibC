/* This file has been put into the public domain by its author. */

#include <errno.h>
#include <fenv.h>
#include <math.h>

/* Sollya generated coefficients for asin */
/* Infinity norm absolute error: 0x1.48e8eade6df16c93c36685fe2a6b754db19e4730fb88c5443p-62 */
static const double POLY_C3_HI = 0x1.55555555555cbp-3;
static const double POLY_C3_LO = -0x1.b12762b7206cp-62;
static const double POLY_C5_HI = 0x1.33333333258dap-4;
static const double POLY_C5_LO = 0x1.fa67b515bfd94p-60;
static const double POLY_C7_HI = 0x1.6db6db774e7bdp-5;
static const double POLY_C7_LO = -0x1.e8ad36345bec8p-59;
static const double POLY_C9 = 0x1.f1c718e444517p-6;
static const double POLY_C11 = 0x1.6e8c09101cba6p-6;
static const double POLY_C13 = 0x1.1c4743e2598a1p-6;
static const double POLY_C15 = 0x1.ca5a48a988944p-7;
static const double POLY_C17 = 0x1.73c871cdd8476p-7;
static const double POLY_C19 = 0x1.6a5764f0b3a76p-7;
static const double POLY_C21 = 0x1.648438f282542p-9;
static const double POLY_C23 = 0x1.88dfa941e5e64p-6;
static const double POLY_C25 = -0x1.8a267bcc1fd33p-6;
static const double POLY_C27 = 0x1.1a42423792acfp-5;

/* Pi / 2 precisely encoded as Double-Double */
static const double PI_O_2_HI = 0x1.921fb54442d18p+0;
static const double PI_O_2_LO = 0x1.1a62633145c07p-54;

double asin(double x)
{
    double abs_x = fabs(x);

    /* --- Special cases and Error Handling --- */

    /* If x is NaN, a NaN shall be returned. */
    if (isnan(x)) {
        return x;
    }

    /* Domain Error: The x argument is finite and is not in the range [-1, 1], or is +/-Inf. */
    if (abs_x > 0x1.0p+0) { /* |x| > 1.0 */
        if (math_errhandling & MATH_ERRNO)
            errno = EDOM;
        if (math_errhandling & MATH_ERREXCEPT)
            feraiseexcept(FE_INVALID);
        return nan("");
    }

    /* Range Error: The value of x is subnormal. */
    if (fpclassify(x) == FP_SUBNORMAL) {
        if (math_errhandling & MATH_ERRNO)
            errno = ERANGE;
        if (math_errhandling & MATH_ERREXCEPT)
            feraiseexcept(FE_UNDERFLOW);
        return x;
    }

    /* If x is +/-0, x shall be returned. */
    if (fpclassify(x) == FP_ZERO) {
        return x;
    }

    /* --- Main Logic --- */

    if (abs_x <= 0x1.0p-1) {     /* |x| <= 0.5 */
        if (abs_x < 0x1.0p-28) { /* |x| < 2^-28 */
            return x;
        }

        /* Standard evaluation for |x| in [2^-28, 0.5] */
        double t = abs_x * abs_x;
        double t_lo = fma(abs_x, abs_x, -t);

        double H, P_lo, P_hi, P_hi_final, sum, y, res;

        H = POLY_C27;
        H = fma(t, H, POLY_C25);
        H = fma(t, H, POLY_C23);
        H = fma(t, H, POLY_C21);
        H = fma(t, H, POLY_C19);
        H = fma(t, H, POLY_C17);
        H = fma(t, H, POLY_C15);
        H = fma(t, H, POLY_C13);
        H = fma(t, H, POLY_C11);
        H = fma(t, H, POLY_C9);

        P_lo = fma(t, H, POLY_C7_LO);
        P_hi = POLY_C7_HI;

        P_lo = fma(t, P_lo, POLY_C5_LO);
        P_hi = fma(t, P_hi, POLY_C5_HI);

        P_lo = fma(t, P_lo, POLY_C3_LO);

        P_hi_final = fma(t, P_hi, POLY_C3_HI);

        /* Compensate for x^2 rounding error */
        sum = fma(t_lo, POLY_C3_HI, t * P_lo);
        y = fma(t, P_hi_final, sum);

        res = fma(abs_x, y, abs_x);
        return (x < 0x0.0p+0) ? -res : res;
    }

    /* |x| in (0.5, 1.0] */
    /* Identity: asin(x) = sign(x) * (pi/2 - 2 * asin(sqrt((1 - |x|) / 2)))
       1.0 - abs_x is mathematically exact by Sterbenz Lemma.
       Division by 2 is an exact exponent shift. */
    double z = (0x1.0p+0 - abs_x) * 0x1.0p-1; /* (1.0 - |x|) * 0.5 */
    double s = sqrt(z);

    /* z is the exact square of our input 's' before the sqrt operation.
       Thus, we do not need t_lo (it is 0.0) and there is no squaring error. */
    double t = z;

    double H, P_lo, P_hi, P_hi_final, sum, y;
    double term1, term2, res;

    H = POLY_C27;
    H = fma(t, H, POLY_C25);
    H = fma(t, H, POLY_C23);
    H = fma(t, H, POLY_C21);
    H = fma(t, H, POLY_C19);
    H = fma(t, H, POLY_C17);
    H = fma(t, H, POLY_C15);
    H = fma(t, H, POLY_C13);
    H = fma(t, H, POLY_C11);
    H = fma(t, H, POLY_C9);

    P_lo = fma(t, H, POLY_C7_LO);
    P_hi = POLY_C7_HI;

    P_lo = fma(t, P_lo, POLY_C5_LO);
    P_hi = fma(t, P_hi, POLY_C5_HI);

    P_lo = fma(t, P_lo, POLY_C3_LO);

    P_hi_final = fma(t, P_hi, POLY_C3_HI);

    /* Evaluate t * P(t). No t_lo needed. */
    sum = t * P_lo;
    y = fma(t, P_hi_final, sum);

    /* We need to compute: pi/2 - 2 * (s + s * y).
       Evaluated carefully as: (PI_O_2_HI - 2*s) + (PI_O_2_LO - 2*s*y) */
    term1 = fma(-0x1.0p+1, s, PI_O_2_HI);     /* fma(-2.0, s, PI_O_2_HI) */
    term2 = fma(-0x1.0p+1 * s, y, PI_O_2_LO); /* fma(-2.0 * s, y, PI_O_2_LO) */
    res = term1 + term2;

    return (x < 0x0.0p+0) ? -res : res;
}
