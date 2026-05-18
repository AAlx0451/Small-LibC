/* This file has been put into the public domain by its author. */

#include <errno.h>
#include <fenv.h>
#include <math.h>

/* 
 * Constants for exp(x)
 * 
 * InvLn2 = 1 / ln(2)
 * Ln2_HI + Ln2_LO = ln(2) to more than 100 bits of precision.
 */
static const double InvLn2 = 0x1.71547652b82fep0; /* 1.4426950408889634 */
static const double Ln2_HI = 0x1.62e42fefa39efp-1; /* 0.693147180559945286226763982995 */
static const double Ln2_LO = 0x1.abc9e3b39803fp-56; /* 2.319046813846299558417e-17 */

/* Sollya coefficients for (e^r - 1 - r - r^2/2) / r^3 */
static const double C0  = 0x1.5555555555555p-3;
static const double C1  = 0x1.5555555555556p-5;
static const double C2  = 0x1.1111111111488p-7;
static const double C3  = 0x1.6c16c16c160ap-10;
static const double C4  = 0x1.a01a019d94482p-13;
static const double C5  = 0x1.a01a01a7b3cf8p-16;
static const double C6  = 0x1.71de465566526p-19;
static const double C7  = 0x1.27e4d9494b289p-22;
static const double C8  = 0x1.ae3256420f313p-26;
static const double C9  = 0x1.1f7696c09e3eep-29;
static const double C10 = 0x1.ad26bb12e5169p-33;

double exp(double x)
{
    double kd, r_hi, r_lo, r2, p, tail, sum_hi, sum_lo, res;
    int k;

    if (isnan(x)) {
        return x;
    }
    if (x > 709.78271289338397) {
        if (math_errhandling & MATH_ERRNO) errno = ERANGE;
        if (math_errhandling & MATH_ERREXCEPT) feraiseexcept(FE_OVERFLOW | FE_INEXACT);
        return HUGE_VAL;
    }
    if (x < -745.1332191019411) {
        if (math_errhandling & MATH_ERRNO) errno = ERANGE;
        if (math_errhandling & MATH_ERREXCEPT) feraiseexcept(FE_UNDERFLOW | FE_INEXACT);
        return 0.0;
    }

    /* Argument reduction: x = k * ln(2) + r */
    kd = round(x * InvLn2);
    k = (int)kd;

    /* 
     * r_hi = x - kd * Ln2_HI. 
     * Using fma ensures this subtraction is exact. 
     */
    r_hi = fma(-kd, Ln2_HI, x);
    r_lo = -kd * Ln2_LO;

    /* Polynomial evaluation using Horner scheme */
    p = C10;
    p = fma(p, r_hi, C9);
    p = fma(p, r_hi, C8);
    p = fma(p, r_hi, C7);
    p = fma(p, r_hi, C6);
    p = fma(p, r_hi, C5);
    p = fma(p, r_hi, C4);
    p = fma(p, r_hi, C3);
    p = fma(p, r_hi, C2);
    p = fma(p, r_hi, C1);
    p = fma(p, r_hi, C0);

    /* tail = r_lo + 0.5 * r_hi^2 + r_hi^3 * p */
    r2 = r_hi * r_hi;
    tail = fma(r2, fma(r_hi, p, 0.5), r_lo);

    /* 
     * Reconstruct e^r = 1.0 + r_hi + tail.
     * We use FastTwoSum for 1.0 + r_hi because |r_hi| <= 0.346... < 1.0.
     */
    sum_hi = 1.0 + r_hi;
    sum_lo = r_hi - (sum_hi - 1.0);
    sum_lo += tail;

    res = sum_hi + sum_lo;

    /* Scale by 2^k */
    return ldexp(res, k);
}
