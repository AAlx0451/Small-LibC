#include <math.h>
#include <stdint.h>
#include <errno.h>

static const double LN2 = 0.693147180559945309417;

double log(double x) {
    union {
        double f;
        uint64_t i;
    } u;

    u.f = x;

    if (x < 0.0) { 
        errno = EDOM;
        return NAN;
    }
    if (x == 0.0) { 
        errno = ERANGE;
        return -HUGE_VAL;
    }
    if (x != x || x == HUGE_VAL) { 
        return x;
    }
    if (x == 1.0) {
        return 0.0;
    }

    int k = ((u.i >> 52) & 0x7FF) - 1023;
    u.i = (u.i & 0x000FFFFFFFFFFFFFULL) | 0x3FF0000000000000ULL;
    double m = u.f;

    double z = (m - 1.0) / (m + 1.0);
    double z2 = z * z;
    double sum = z;
    double term = z;
    
    term *= z2; sum += term / 3.0;
    term *= z2; sum += term / 5.0;
    term *= z2; sum += term / 7.0;
    term *= z2; sum += term / 9.0;

    return 2.0 * sum + k * LN2;
}
