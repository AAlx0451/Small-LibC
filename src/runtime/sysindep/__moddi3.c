#include <limits.h>

long long __moddi3(long long n, long long d) {
    if (d == 0) {
        return 0;
    }

    if (n == LLONG_MIN && d == -1) {
        return 0;
    }

    int n_sign = (n < 0) ? -1 : 1;

    unsigned long long un = (n < 0) ? -(unsigned long long)n : (unsigned long long)n;
    unsigned long long ud = (d < 0) ? -(unsigned long long)d : (unsigned long long)d;

    unsigned long long remainder = 0;
    const int BITS = sizeof(unsigned long long) * CHAR_BIT;

    for (int i = BITS - 1; i >= 0; i--) {
        remainder <<= 1;
        remainder |= (un >> i) & 1;
        if (remainder >= ud) {
            remainder -= ud;
        }
    }

    if (n_sign == -1) {
        return -(long long)remainder;
    } else {
        return (long long)remainder;
    }
}
