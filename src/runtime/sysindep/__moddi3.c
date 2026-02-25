#include "runtime.h"
#include <limits.h>

long long __moddi3(long long n, long long d) {
    unsigned long long un, ud, remainder;
    int n_sign, i;
    if(d == 0)
        return 0; // Undefined
    if(n == LLONG_MIN && d == -1)
        return 0;

    n_sign = (n < 0) ? -1 : 1;
    un = (n < 0) ? -(unsigned long long)n : (unsigned long long)n;
    ud = (d < 0) ? -(unsigned long long)d : (unsigned long long)d;
    remainder = 0;

    for(i = 63; i >= 0; i--) {
        remainder = (remainder << 1) | ((un >> i) & 1);
        if(remainder >= ud) {
            remainder -= ud;
        }
    }
    return (n_sign < 0) ? -(long long)remainder : (long long)remainder;
}
