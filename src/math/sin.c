#include <math.h>
#include <errno.h>
#include <stdint.h>

/* Constants for Cody-Waite range reduction */
static const double M_PI_2 = 1.57079632679489661923;
static const double C1 = 1.57079632673412561417e+00; /* 53 bits of pi/2 */
static const double C2 = 6.07710050650619224932e-11; /* next bits */
static const double C3 = 1.90821492927058770002e-20; /* last bits */

/* Polynomial coefficients for sin(x) approx x + S1*x^3 + ... */
static const double S1 = -1.66666666666666324348e-01;
static const double S2 =  8.33333333332248946124e-03;
static const double S3 = -1.98412698298579493134e-04;
static const double S4 =  2.75573137070700676789e-06;
static const double S5 = -2.50507602534068634195e-08;
static const double S6 =  1.58969099521155010221e-10;

/* Polynomial coefficients for cos(x) approx 1 + C1*x^2 + ... */
static const double CC1 =  4.16666666666666019037e-02;
static const double CC2 = -1.38888888888741095749e-03;
static const double CC3 =  2.48015872894767294178e-05;
static const double CC4 = -2.75573143513906633035e-07;
static const double CC5 =  2.08757232129817482790e-09;
static const double CC6 = -1.13596475577881948265e-11;

static double __kernel_sin(double x, double y) {
    double z = x * x;
    double v = z * x;
    double r = S2 + z * (S3 + z * (S4 + z * (S5 + z * S6)));
    if (y == 0.0) return x + v * (S1 + z * r);
    return x - ((z * (0.5 * y - v * r) - y) - v * S1);
}

static double __kernel_cos(double x, double y) {
    double z = x * x;
    double r = z * (CC1 + z * (CC2 + z * (CC3 + z * (CC4 + z * (CC5 + z * CC6)))));
    double hz = 0.5 * z;
    double w = 1.0 - hz;
    return w + (((1.0 - w) - hz) + (z * r - x * y));
}

double sin(double x) {
    double y[2], fn;
    int n, k;

    /* Handle Infinite / NaN */
    if (!isfinite(x)) {
        if (isnan(x)) return x;
        errno = EDOM;
        return NAN;
    }
    if (x == 0.0) return x;

    /* Argument reduction: x = n * pi/2 + r */
    /* Using Cody-Waite reduction logic inline */
    fn = x / M_PI_2;
    n = (int)(fn + (x < 0 ? -0.5 : 0.5));
    y[0] = x - n * C1;
    y[0] -= n * C2;
    y[0] -= n * C3;
    y[1] = 0.0; /* Extended precision tail if needed */

    k = n & 3; /* Quadrant selection */

    switch (k) {
        case 0: return  __kernel_sin(y[0], y[1]);
        case 1: return  __kernel_cos(y[0], y[1]);
        case 2: return -__kernel_sin(y[0], y[1]);
        default: return -__kernel_cos(y[0], y[1]);
    }
}
