#include <stdint.h>
#include <string.h>

double ceil(double x)
{
    uint64_t i;
    uint64_t sign;
    int exponent;
    uint64_t frac_mask;

    memcpy(&i, &x, 8);
    sign = i & 0x8000000000000000ULL;
    exponent = (int)((i >> 52) & 0x7FF) - 1023;

    if (exponent < 0) {
        if ((i & 0x7FFFFFFFFFFFFFFFULL) == 0) {

        } else if (sign) {
            i = 0x8000000000000000ULL;
        } else {
            i = 0x3FF0000000000000ULL;
        }
    } else if (exponent < 52) {
        frac_mask = (1ULL << (52 - exponent)) - 1;
        if ((i & frac_mask) != 0) {
            if (!sign) {
                i += 1ULL << (52 - exponent);
            }
            i &= ~frac_mask;
        }
    }

    memcpy(&x, &i, 8);
    return x;
}
