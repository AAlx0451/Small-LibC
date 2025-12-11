#include <math.h>
#include <errno.h>

double cosh(double x) {
    double e;
    if (!isfinite(x)) {
        if (isnan(x)) return x;
        return HUGE_VAL; /* Inf returns Inf */
    }
    x = (x < 0) ? -x : x;
    
    if (x > 710.0) {
        errno = ERANGE;
        return HUGE_VAL;
    }

    e = exp(x);
    return 0.5 * (e + 1.0/e);
}
