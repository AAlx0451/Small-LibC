#include <math.h>
#include <stdint.h>
#include <errno.h>

static const long double LN2_HI = 6.93147180369123816490e-01L;
static const long double LN2_LO = 1.90821492927058770002e-10L;
static const long double INV_LN2 = 1.4426950408889634073599246810018921374266L;

double exp(double x) {
    union {
        double f;
        uint64_t i;
    } u;

    if (x != x) return x; 
    if (x == HUGE_VAL) return HUGE_VAL; 
    if (x == -HUGE_VAL) return 0.0; 

    if (x > 709.782712893384) {
        errno = ERANGE;
        return HUGE_VAL;
    }
    if (x < -708.3964185322641) {
        errno = ERANGE;
        return 0.0;
    }

    long double k_ld = floorl(x * INV_LN2 + 0.5L);
    int k = (int)k_ld;
    long double r_ld = x - k_ld * LN2_HI - k_ld * LN2_LO;
    double r = (double)r_ld;

    double term = r;
    double sum = 1.0 + term;
    term *= r / 2.0;
    sum += term;
    term *= r / 3.0;
    sum += term;
    term *= r / 4.0;
    sum += term;
    term *= r / 5.0;
    sum += term;
    
    u.i = (uint64_t)(1023 + k) << 52;
    
    return sum * u.f;
}
