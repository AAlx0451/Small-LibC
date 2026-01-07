#include <limits.h>

long long __divdi3(long long n, long long d) {
    if (d == 0) return 0; // Division by zero undefined
    if (n == LLONG_MIN && d == -1) return LLONG_MIN; 

    int sign = (n < 0) ^ (d < 0) ? -1 : 1;
    unsigned long long un = (n < 0) ? -(unsigned long long)n : (unsigned long long)n;
    unsigned long long ud = (d < 0) ? -(unsigned long long)d : (unsigned long long)d;

    unsigned long long quotient = 0;
    unsigned long long remainder = 0;
    
    for (int i = 63; i >= 0; i--) {
        remainder = (remainder << 1) | ((un >> i) & 1);
        if (remainder >= ud) {
            remainder -= ud;
            quotient |= (1ULL << i);
        }
    }
    return (sign < 0) ? -(long long)quotient : (long long)quotient;
}
