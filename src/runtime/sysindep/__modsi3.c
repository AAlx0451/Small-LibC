#include "runtime.h"
#include <limits.h>

int __modsi3(int n, int d) {
    int n_sign;
    int i;
    unsigned int un, ud, remainder;

    if(d == 0)
        return 0;
    if(n == INT_MIN && d == -1)
        return 0;

    n_sign = (n < 0) ? -1 : 1;
    un = (n < 0) ? -(unsigned int)n : (unsigned int)n;
    ud = (d < 0) ? -(unsigned int)d : (unsigned int)d;
    remainder = 0;

    for(i = 31; i >= 0; i--) {
        remainder = (remainder << 1) | ((un >> i) & 1);
        if(remainder >= ud) {
            remainder -= ud;
        }
    }

    return (n_sign < 0) ? -(int)remainder : (int)remainder;
}
