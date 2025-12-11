#include <math.h>
#include <errno.h>

double asin(double x) {
    if (x < -1.0 || x > 1.0) {
        errno = EDOM;
        return NAN;
    }
    /* asin(x) = atan2(x, sqrt(1-x*x)) */
    return atan2(x, sqrt(1.0 - x*x));
}
