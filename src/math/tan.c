#include <math.h>
#include <errno.h>
#include <stdint.h>

static const double M_PI_2 = 1.57079632679489661923;
static const double C1 = 1.57079632673412561417e+00;
static const double C2 = 6.07710050650619224932e-11;
static const double C3 = 1.90821492927058770002e-20;

/* Coefficients for tan(x) */
static const double T[] = {
  3.33333333333334091986e-01,
  1.33333333333201242699e-01,
  5.39682539762265521005e-02,
  2.18694882948595424599e-02,
  8.86323982359930005737e-03,
  3.59207910759131235356e-03,
  1.45620945432529025516e-03,
  5.88041240820264094204e-04,
  2.46463134818469906812e-04,
  7.81794442939557092300e-05,
  7.14072491382608190305e-05,
  -1.85586374855275456654e-05,
  2.59073051863633712884e-05
};

static double __kernel_tan(double x, double y, int iy) {
    double z, r, w;
    double osign = 1.0;

    if (iy == 1) { /* cot definition used for high precision if needed, here simplified */
        if (x >= 0) { x = -x; y = -y; }
        else osign = -1.0;
    }
    
    z = x * x;
    
    /* Pade approximation or large polynomial */
    /* Using a simpler high-order polynomial for compactness here */
    r = T[1] + z*(T[2] + z*(T[3] + z*(T[4] + z*(T[5] + z*(T[6] + z*(T[7] + z*(T[8] + z*(T[9] + z*(T[10] + z*(T[11] + z*T[12]))))))))));
    r = T[0] + z * r;
    w = x + x * (z * r);

    if (iy == 1) {
        /* Compute -1.0 / (x + ...) carefully */
        double num = -1.0;
        return (num / w) * osign; /* Simplified cot */
    }
    return w;
}

double tan(double x) {
    double y[2], fn;
    int n;

    if (!isfinite(x)) {
        if (isnan(x)) return x;
        errno = EDOM;
        return NAN;
    }
    if (x == 0.0) return x;

    fn = x / M_PI_2;
    n = (int)(fn + (x < 0 ? -0.5 : 0.5));
    y[0] = x - n * C1;
    y[0] -= n * C2;
    y[0] -= n * C3;
    y[1] = 0.0;

    if ((n & 1)) {
        /* odd quadrant: -cot(x) or similar */
        return -__kernel_tan(y[0], y[1], 1); 
    } else {
        return __kernel_tan(y[0], y[1], 0);
    }
}
