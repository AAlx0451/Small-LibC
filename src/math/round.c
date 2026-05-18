#include <math.h>
#include <stdint.h>
#include <string.h>

double round(double x) {
    uint64_t i;
    uint64_t sign;
    int exponent;
    uint64_t mask;

    memcpy(&i, &x, 8);
    sign = i & 0x8000000000000000ULL;
    i &= 0x7FFFFFFFFFFFFFFFULL;
    exponent = (int)(i >> 52) - 1023;

    if (exponent < 0) {
        if (exponent == -1) {
            i = 0x3FF0000000000000ULL;
        } else {
            i = 0;
        }
    } else if (exponent < 52) {
        i += 1ULL << (51 - exponent);
        mask = ~((1ULL << (52 - exponent)) - 1);
        i &= mask;
    }

    i |= sign;
    memcpy(&x, &i, 8);
    return x;
}
