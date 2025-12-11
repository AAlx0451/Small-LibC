#include <math.h>

#define M_PI 3.14159265358979323846
#define M_PI_2 1.57079632679489661923

static double _atan_kernel(double x) {
    /* Approximate atan(x) for x in [0, 1] */
    double x2 = x * x;
    return x * (1.0 + x2 * (-0.3333333333332993 + x2 * (0.1999999999975505 +
           x2 * (-0.1428571426677133 + x2 * (0.1111111016427351 +
           x2 * (-0.0909088713346917))))));
}

double atan2(double y, double x) {
    if (isnan(x) || isnan(y)) return NAN;

    /* Handle origin */
    if (x == 0.0 && y == 0.0) return (signbit(y)) ? -0.0 : 0.0; // Strictly implementation defined, but standard often matches sign

    /* Handle axes exactly */
    if (y == 0.0) return (x >= 0.0) ? (signbit(y) ? -0.0 : 0.0) : (signbit(y) ? -M_PI : M_PI);
    if (x == 0.0) return (y > 0.0) ? M_PI_2 : -M_PI_2;

    /* Handle Inf */
    if (!isfinite(x) || !isfinite(y)) {
        if (x == HUGE_VAL) return (y > 0 && !isinf(y)) ? 0.0 : (y > 0 ? M_PI/4 : -M_PI/4); // Simplify complex Inf logic
        if (x == -HUGE_VAL) return (y > 0) ? 3*M_PI/4 : -3*M_PI/4;
        if (y == HUGE_VAL) return M_PI_2;
        if (y == -HUGE_VAL) return -M_PI_2;
    }

    /* Range reduction */
    double z = y / x;
    double az = fabs(z);
    double val;

    if (az > 1.0) {
        val = M_PI_2 - _atan_kernel(1.0 / az);
    } else {
        val = _atan_kernel(az);
    }

    /* Quadrant placement */
    if (x > 0.0) {
        return (y > 0.0) ? val : -val;
    } else {
        return (y > 0.0) ? (M_PI - val) : (val - M_PI);
    }
}
