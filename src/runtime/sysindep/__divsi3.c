#include <limits.h>

int __divsi3(int n, int d) {
    if (d == 0) {
        return 0;
    }

    if (n == INT_MIN && d == -1) {
        return INT_MIN;
    }

    int sign = 1;
    if ((n < 0) != (d < 0)) {
        sign = -1;
    }

    unsigned int un = (n < 0) ? -(unsigned int)n : (unsigned int)n;
    unsigned int ud = (d < 0) ? -(unsigned int)d : (unsigned int)d;

    unsigned int quotient = 0;
    unsigned int remainder = 0;
    const int BITS = sizeof(unsigned int) * CHAR_BIT;

    for (int i = BITS - 1; i >= 0; i--) {
        remainder <<= 1;
        remainder |= (un >> i) & 1;
        if (remainder >= ud) {
            remainder -= ud;
            quotient |= (1U << i);
        }
    }

    if (sign == -1) {
        return -(int)quotient;
    } else {
        return (int)quotient;
    }
}
