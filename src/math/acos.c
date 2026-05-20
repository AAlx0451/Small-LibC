/* This file has been put into the public domain by its author. */

#include <math.h>
#include <errno.h>
#include <fenv.h>

/* Sollya coefficients from asin */
/* Infinity norm absolute error: 0x1.48e8eade6df16c93c36685fe2a6b754db19e4730fb88c5443p-62 */
static const double POLY_C3_HI =  0x1.55555555555cbp-3;
static const double POLY_C3_LO = -0x1.b12762b7206cp-62;
static const double POLY_C5_HI =  0x1.33333333258dap-4;
static const double POLY_C5_LO =  0x1.fa67b515bfd94p-60;
static const double POLY_C7_HI =  0x1.6db6db774e7bdp-5;
static const double POLY_C7_LO = -0x1.e8ad36345bec8p-59;
static const double POLY_C9    =  0x1.f1c718e444517p-6;
static const double POLY_C11   =  0x1.6e8c09101cba6p-6;
static const double POLY_C13   =  0x1.1c4743e2598a1p-6;
static const double POLY_C15   =  0x1.ca5a48a988944p-7;
static const double POLY_C17   =  0x1.73c871cdd8476p-7;
static const double POLY_C19   =  0x1.6a5764f0b3a76p-7;
static const double POLY_C21   =  0x1.648438f282542p-9;
static const double POLY_C23   =  0x1.88dfa941e5e64p-6;
static const double POLY_C25   = -0x1.8a267bcc1fd33p-6;
static const double POLY_C27   =  0x1.1a42423792acfp-5;

/* Pi and Pi/2 precise Double-Double representations */
static const double PI_O_2_HI =  0x1.921fb54442d18p+0;
static const double PI_O_2_LO =  0x1.1a62633145c07p-54;
static const double PI_HI     =  0x1.921fb54442d18p+1;
static const double PI_LO     =  0x1.1a62633145c07p-53;

double acos(double x) {
    /* acos(NaN) is NaN */
    if (isnan(x)) {
        return x;
    }

    double abs_x = fabs(x);

    /* x is not in [-1, 1] */
    if (abs_x > 1.0) {
        if (math_errhandling & MATH_ERRNO) errno = EDOM;
        if (math_errhandling & MATH_ERREXCEPT) feraiseexcept(FE_INVALID);

        return nan("");
    }

    /* acos(1.0) = 0.0 */
    if (x == 1.0) {
        return 0.0;
    }
    
    double H, P_lo, P_hi, P_hi_final, sum;

    if (abs_x <= 0.5) {
        /* acos(x) = pi/2 - asin(x) */
        double t = x * x;
        double t_lo = fma(x, x, -t);
        double y, z, err;

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
        sum = fma(t_lo, POLY_C3_HI, t * P_lo);
        y = fma(t, P_hi_final, sum);

        /* Fast2Sum to capture exact rounding error of PI_O_2_HI - x.
           Condition |PI_O_2_HI| > |x| is strictly guaranteed (1.57 > 0.5). */
        z = PI_O_2_HI - x;
        err = (PI_O_2_HI - z) - x;

        /* reassemble: z + err + PI_O_2_LO - x * P(x^2) */
        return z + fma(-x, y, err + PI_O_2_LO);

    } else if (abs_x <= 1.0) {
        if (x > 0.0) {
            /* Interval (0.5, 1.0]: acos(x) = 2 * asin(sqrt((1 - x) / 2)) 
               (1.0 - x) is exact by Sterbenz Lemma. */
            double z_val = (1.0 - x) * 0.5;
            double s = sqrt(z_val);
            
            /* s is the sqrt of z_val. Therefore, z_val is the exact mathematical 
               square of s prior to sqrt rounding. t_lo is exactly 0.0! */
            double t = z_val;
            double y;

            /* Inlined EVAL_POLY(t, 0.0, y) */
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
            sum = fma(0.0, POLY_C3_HI, t * P_lo);
            y = fma(t, P_hi_final, sum);

            /* Return 2*s + 2*s*y safely */
            return fma(2.0 * s, y, 2.0 * s);
        } else {
            /* Interval [-1.0, -0.5): acos(x) = pi - 2 * asin(sqrt((1 + x) / 2)) 
               x is negative, so 1+x evaluates exactly without catastrophic cancellation. */
            double z_val = (1.0 + x) * 0.5;
            double s = sqrt(z_val);
            double t = z_val;
            double y, z2, err;

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
            sum = fma(0.0, POLY_C3_HI, t * P_lo);
            y = fma(t, P_hi_final, sum);

            /* Fast2Sum to capture error of PI_HI - 2*s. 
               |PI_HI| (3.14) > |2s| (<=1.0) is guaranteed. */
            z2 = PI_HI - 2.0 * s;
            err = (PI_HI - z2) - 2.0 * s;

            /* return (PI_HI - 2s) + PI_LO - 2s*y + err */
            return z2 + fma(-2.0 * s, y, err + PI_LO);
        }
    }

    /* fallback */
    return nan("");
}
