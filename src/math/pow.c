#include <math.h>
#include <errno.h>
#include <limits.h>

/* Helper to check if y is an odd integer */
static int is_odd_int(double y) {
    double iptr;
    if (modf(y, &iptr) != 0.0) return 0; /* not integer */
    long long i = (long long)y;
    return (i & 1);
}

double pow(double x, double y) {
    double result;
    int y_is_int = (x < 0.0) ? (floor(y) == y) : 0;

    /* 1. Handle y == 0 */
    if (y == 0.0) return 1.0;

    /* 2. Handle NaN */
    if (isnan(x) || isnan(y)) return NAN;

    /* 3. Handle x == 1 */
    if (x == 1.0) return 1.0;

    /* 4. Handle y == Inf */
    if (!isfinite(y)) {
        if (y > 0) {
            if (fabs(x) > 1.0) return HUGE_VAL;
            if (fabs(x) < 1.0) return 0.0;
            return 1.0; /* 1.0^inf = 1.0 */
        } else {
            /* y == -Inf */
            if (fabs(x) > 1.0) return 0.0;
            if (fabs(x) < 1.0) return HUGE_VAL;
            return 1.0;
        }
    }

    /* 5. Handle x == 0 */
    if (x == 0.0) {
        if (y > 0.0) {
            /* 0^odd = 0, 0^even = 0 */
            /* Check for sign of zero handled by hardware usually, but: */
            if (is_odd_int(y) && signbit(x)) return -0.0;
            return 0.0;
        } else {
            /* 0^-y = Inf */
            errno = ERANGE;
            if (is_odd_int(y) && signbit(x)) return -HUGE_VAL;
            return HUGE_VAL;
        }
    }

    /* 6. Handle x == Inf */
    if (!isfinite(x)) {
        if (x > 0) {
            return (y > 0) ? HUGE_VAL : 0.0;
        } else {
            /* x == -Inf */
            if (y > 0) {
                if (is_odd_int(y)) return -HUGE_VAL;
                return HUGE_VAL;
            } else {
                /* y < 0 */
                if (is_odd_int(y)) return -0.0;
                return 0.0;
            }
        }
    }

    /* 7. Handle x < 0 finite */
    if (x < 0.0) {
        if (!y_is_int) {
            errno = EDOM;
            return NAN;
        }
        /* pow(x,y) = sign * exp(y * log(|x|)) */
        result = exp(y * log(-x));
        if (is_odd_int(y)) {
            return -result;
        }
        return result;
    }

    /* 8. Standard case x > 0 */
    return exp(y * log(x));
}
