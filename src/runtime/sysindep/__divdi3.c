#include <limits.h>

long long __divdi3(long long n, long long d) {
    if (d == 0) {
        return 0;
    }

    if (n == LLONG_MIN && d == -1) {
        return LLONG_MIN;
    }

    int sign = 1;
    if ((n < 0) != (d < 0)) {
        sign = -1;
    }

    unsigned long long un = (n < 0) ? -(unsigned long long)n : (unsigned long long)n;
    unsigned long long ud = (d < 0) ? -(unsigned long long)d : (unsigned long long)d;

    unsigned long long quotient = 0;
    unsigned long long remainder = 0;
    const int BITS = sizeof(unsigned long long) * CHAR_BIT;

    for (int i = BITS - 1; i >= 0; i--) {
        remainder <<= 1;
        remainder |= (un >> i) & 1;
        if (remainder >= ud) {
            remainder -= ud;
            quotient |= (1ULL << i);
        }
    }

    if (sign == -1) {
        return -(long long)quotient;
    } else {
        return (long long)quotient;
    }
}
