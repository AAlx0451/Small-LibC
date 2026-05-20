/* This file has been put into the public domain by its author. */

#include <errno.h>
#include <fenv.h>
#include <math.h>

/* Sollya coefficients */
static const double ATAN_P0 = -0x1.5555555555555p-2;
static const double ATAN_P1 = 0x1.9999999999429p-3;
static const double ATAN_P2 = -0x1.249249243d5b1p-3;
static const double ATAN_P3 = 0x1.c71c7188a8d81p-4;
static const double ATAN_P4 = -0x1.745d0b99503d7p-4;
static const double ATAN_P5 = 0x1.3b126bad6e731p-4;
static const double ATAN_P6 = -0x1.10fae1bfa2713p-4;
static const double ATAN_P7 = 0x1.dfecac12a767ep-5;
static const double ATAN_P8 = -0x1.a0b7ad57c87c6p-5;
static const double ATAN_P9 = 0x1.41b1c5634b367p-5;
static const double ATAN_P10 = -0x1.3ae34f45d4e06p-6;

/* pi/2 and pi/4 in double-double */
static const double PIO2_HI = 0x1.921fb54442d18p+0;
static const double PIO2_LO = 0x1.1a62633145c07p-54;
static const double PIO4_HI = 0x1.921fb54442d18p-1;
static const double PIO4_LO = 0x1.1a62633145c07p-55;

double atan(double x)
{
    double abs_x = fabs(x);

    /* --- Special cases and Error Handling --- */

    /* If x is NaN, a NaN shall be returned. */
    if (isnan(x)) {
        return x + x; /* Raise invalid if sNaN */
    }

    /* If x is +/-0, x shall be returned. */
    if (x == 0x0.0p+0) {
        return x;
    }

    /* If x is +/-Inf, +/-pi/2 shall be returned. */
    if (isinf(x)) {
        return (x < 0x0.0p+0) ? -PIO2_HI : PIO2_HI;
    }

    /* Range Error: The value of x is subnormal. */
    if (fpclassify(x) == FP_SUBNORMAL) {
        if (math_errhandling & MATH_ERRNO)
            errno = ERANGE;
        if (math_errhandling & MATH_ERREXCEPT)
            feraiseexcept(FE_UNDERFLOW);
        return x;
    }

    /* --- Main Logic --- */

    int id;
    double z, z2, p, res_hi, res_lo;

    /* Range reduction boundaries corresponding to tan(pi/8) and tan(3pi/8) */
    if (abs_x < 0x1.a827999fcef31p-2) { /* ~ 0.4142135623730950 */
        id = 0;
        z = abs_x;
    } else if (abs_x < 0x1.3504f333f9de6p+1) { /* ~ 2.414213562373095 */
        id = 1;
        z = (abs_x - 0x1.0p+0) / (abs_x + 0x1.0p+0);
    } else {
        id = 2;
        z = -0x1.0p+0 / abs_x;
    }

    z2 = z * z;
    p = ATAN_P10;
    p = fma(p, z2, ATAN_P9);
    p = fma(p, z2, ATAN_P8);
    p = fma(p, z2, ATAN_P7);
    p = fma(p, z2, ATAN_P6);
    p = fma(p, z2, ATAN_P5);
    p = fma(p, z2, ATAN_P4);
    p = fma(p, z2, ATAN_P3);
    p = fma(p, z2, ATAN_P2);
    p = fma(p, z2, ATAN_P1);
    p = fma(p, z2, ATAN_P0);

    p = fma(z2 * p, z, z);

    /* Reconstruct result via Double-Double arithmetic */
    if (id == 0) {
        res_hi = p;
    } else if (id == 1) {
        res_hi = PIO4_HI + p;
        res_lo = PIO4_LO + (PIO4_HI - res_hi + p);
        res_hi = res_hi + res_lo;
    } else {
        res_hi = PIO2_HI + p;
        res_lo = PIO2_LO + (PIO2_HI - res_hi + p);
        res_hi = res_hi + res_lo;
    }

    /* Apply the original sign */
    return (x < 0x0.0p+0) ? -res_hi : res_hi;
}
