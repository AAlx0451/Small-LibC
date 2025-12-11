#include <limits.h>

unsigned long long __umoddi3(unsigned long long n, unsigned long long d) {
    if (d == 0) {
        return n;
    }

    unsigned long long remainder = 0;
    const int BITS = sizeof(unsigned long long) * CHAR_BIT;

    for (int i = BITS - 1; i >= 0; i--) {
        remainder <<= 1;
        remainder |= (n >> i) & 1;
        if (remainder >= d) {
            remainder -= d;
        }
    }

    return remainder;
}
