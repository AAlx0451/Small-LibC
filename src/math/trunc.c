#include <math.h>
#include <stdint.h>
#include <string.h>

double trunc(double x)
{
    uint64_t i;
    int exponent;
    uint64_t mask;

    memcpy(&i, &x, 8);
    exponent = (int)((i >> 52) & 0x7FF) - 1023;

    if (exponent < 0) {
        i &= 0x8000000000000000ULL;
    } else if (exponent < 52) {
        mask = ~((1ULL << (52 - exponent)) - 1);
        i &= mask;
    }

    memcpy(&x, &i, 8);
    return x;
}
