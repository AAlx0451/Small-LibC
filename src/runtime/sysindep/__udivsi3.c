#include <stdio.h>
#include <limits.h>

unsigned int __udivsi3(unsigned int n, unsigned int d) {
    if (d == 0) {
        return 0;
    }
    unsigned int quotient = 0;
    unsigned int remainder = 0;
    const int BITS = sizeof(unsigned int) * CHAR_BIT;
    for (int i = BITS - 1; i >= 0; i--) {
        remainder <<= 1;
        remainder |= (n >> i) & 1;
        if (remainder >= d) {
            remainder -= d;
            quotient |= (1U << i);
        }
    }
    
    return quotient;
}
