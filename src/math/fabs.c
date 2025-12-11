#include <math.h>
#include <stdint.h>

double fabs(double x) {
    union {
        double f;
        uint64_t i;
    } u;

    u.f = x;
    u.i &= 0x7FFFFFFFFFFFFFULL;
    return u.f;
}
