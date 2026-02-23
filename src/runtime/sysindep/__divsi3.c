#include "runtime.h"
#include <limits.h>

int __divsi3(int n, int d) {
    if(d == 0)
        return 0;
    if(n == INT_MIN && d == -1)
        return INT_MIN;

    int sign = (n < 0) ^ (d < 0) ? -1 : 1;
    unsigned int un = (n < 0) ? -(unsigned int)n : (unsigned int)n;
    unsigned int ud = (d < 0) ? -(unsigned int)d : (unsigned int)d;

    unsigned int quotient = 0;
    unsigned int remainder = 0;

    for(int i = 31; i >= 0; i--) {
        remainder = (remainder << 1) | ((un >> i) & 1);
        if(remainder >= ud) {
            remainder -= ud;
            quotient |= (1U << i);
        }
    }
    return (sign < 0) ? -(int)quotient : (int)quotient;
}
