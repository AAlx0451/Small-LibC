#include <limits.h>

int __modsi3(int n, int d) {
    if (d == 0) {
        return 0;
    }

    if (n == INT_MIN && d == -1) {
        return 0;
    }

    int n_sign = (n < 0) ? -1 : 1;

    unsigned int un = (n < 0) ? -(unsigned int)n : (unsigned int)n;
    unsigned int ud = (d < 0) ? -(unsigned int)d : (unsigned int)d;

    unsigned int remainder = 0;
    const int BITS = sizeof(unsigned int) * CHAR_BIT;

    for (int i = BITS - 1; i >= 0; i--) {
        remainder <<= 1;
        remainder |= (un >> i) & 1;
        if (remainder >= ud) {
            remainder -= ud;
        }
    }

    if (n_sign == -1) {
        return -(int)remainder;
    } else {
        return (int)remainder;
    }
}
