#include <limits.h>

unsigned long long __udivdi3(unsigned long long n, unsigned long long d) {
    if(d == 0)
        return 0;

    unsigned long long quotient = 0;
    unsigned long long remainder = 0;

    for(int i = 63; i >= 0; i--) {
        remainder = (remainder << 1) | ((n >> i) & 1);
        if(remainder >= d) {
            remainder -= d;
            quotient |= (1ULL << i);
        }
    }
    return quotient;
}
