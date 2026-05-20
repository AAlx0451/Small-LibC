/* This file has been put into the public domain by its author. */

#include <math.h>
#include <errno.h>
#include <fenv.h>

static const double PI_HI     = 0x1.921fb54442d18p+1;  /* 3.1415926535897931160e+00 */
static const double PI_LO     = 0x1.1a62633145c07p-53; /* 1.2246467991473532072e-16 */
static const double PIO2_HI   = 0x1.921fb54442d18p+0;
static const double PI_3O4_HI = 0x1.2d97c7f3321d2p+1;  /* 3*pi/4 */
static const double PIO4_HI   = 0x1.921fb54442d18p-1;

double atan2(double y, double x) {
    /* Special cases and Error Handling */

    /* If either x or y is NaN, a NaN shall be returned. */
    if (isnan(x) || isnan(y)) {
        return x + y; /* Returns NaN and reliably raises invalid if sNaN */
    }

    /* If y is +/-0 */
    if (y == 0x0.0p+0) {
        /* If x is < 0 or -0, +/-pi shall be returned. */
        if (signbit(x)) {
            return copysign(PI_HI, y);
        }
        /* If x is > 0 or +0, +/-0 shall be returned. */
        return y;
    }

    /* If x is +/-0 (and y is non-zero) */
    if (x == 0x0.0p+0) {
        /* If y is < 0, -pi/2 shall be returned.
           If y is > 0, pi/2 shall be returned. */
        return copysign(PIO2_HI, y);
    }

    /* If y is +/-Inf */
    if (isinf(y)) {
        if (isinf(x)) {
            /* If y is +/-Inf and x is -Inf, +/-3pi/4 shall be returned. */
            if (signbit(x)) {
                return copysign(PI_3O4_HI, y);
            }
            /* If y is +/-Inf and x is +Inf, +/-pi/4 shall be returned. */
            return copysign(PIO4_HI, y);
        }
        /* For finite values of x, if y is +/-Inf, +/-pi/2 shall be returned. */
        return copysign(PIO2_HI, y);
    }

    /* If x is +/-Inf (and y is finite and non-zero) */
    if (isinf(x)) {
        /* For finite values of +/-y > 0, if x is -Inf, +/-pi shall be returned. */
        if (signbit(x)) {
            return copysign(PI_HI, y);
        }
        /* For finite values of +/-y > 0, if x is +Inf, +/-0 shall be returned. */
        return copysign(0x0.0p+0, y);
    }

    double ax = fabs(x);
    double ay = fabs(y);
    
    /* General evaluation */
    double z = atan(ay / ax);

    /* Supplement angle if x is negative */
    if (signbit(x)) {
        double hi = PI_HI - z;
        double lo = PI_LO + ((PI_HI - hi) - z);
        z = hi + lo;
    }

    /* Apply the original sign of y */
    double res = copysign(z, y);

    /* Range Error: The result underflows. */
    if (fpclassify(res) == FP_SUBNORMAL) {
        if (math_errhandling & MATH_ERRNO) errno = ERANGE;
        if (math_errhandling & MATH_ERREXCEPT) feraiseexcept(FE_UNDERFLOW);
    }

    return res;
}
