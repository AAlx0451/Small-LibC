#include <limits.h>

unsigned long long __udivdi3(unsigned long long n, unsigned long long d) {
    if (d == 0) {
        return 0;
    }

    unsigned long long quotient = 0;
    unsigned long long remainder = 0;
    const int BITS = sizeof(unsigned long long) * CHAR_BIT;

    for (int i = BITS - 1; i >= 0; i--) {
        remainder <<= 1;
        remainder |= (n >> i) & 1;
        if (remainder >= d) {
            remainder -= d;
            quotient |= (1ULL << i);
        }
    }

    return quotient;
}
