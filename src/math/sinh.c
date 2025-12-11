#include <math.h>
#include <errno.h>

double sinh(double x) {
    double e, ei;
    if (!isfinite(x)) return x; /* NaN or Inf */
    if (x == 0.0) return x;
    
    if (x < 0) {
        /* sinh(-x) = -sinh(x) */
        return -sinh(-x); 
    }

    if (x > 710.0) { /* Check boundary for exp overflow roughly */
        errno = ERANGE;
        return HUGE_VAL;
    }

    e = exp(x);
    ei = 1.0 / e;
    return 0.5 * (e - ei);
}
