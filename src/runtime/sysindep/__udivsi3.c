#include "runtime.h"
#include <limits.h>

unsigned int __udivsi3(unsigned int n, unsigned int d) {
    unsigned int quotient = 0;
    unsigned int remainder = 0;
    int i;
    if(d == 0)
        return 0;

    for(i = 31; i >= 0; i--) {
        remainder = (remainder << 1) | ((n >> i) & 1);
        if(remainder >= d) {
            remainder -= d;
            quotient |= (1U << i);
        }
    }
    return quotient;
}
