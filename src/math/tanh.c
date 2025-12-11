#include <math.h>

double tanh(double x) {
    double e, e2;
    if (!isfinite(x)) {
        if (isnan(x)) return x;
        return (x > 0) ? 1.0 : -1.0;
    }
    
    if (x > 20.0) return 1.0;
    if (x < -20.0) return -1.0;
    
    /* tanh(x) = (exp(2x) - 1) / (exp(2x) + 1) */
    e = exp(x);
    e2 = e * e;
    return (e2 - 1.0) / (e2 + 1.0);
}
