#include <stdio.h>
#include <limits.h>

unsigned int __umodsi3(unsigned int n, unsigned int d) {
    if (d == 0) {
        return n;
    }
    unsigned int remainder = 0;
    const int BITS = sizeof(unsigned int) * CHAR_BIT;

    for (int i = BITS - 1; i >= 0; i--) {
        remainder <<= 1;
        remainder |= (n >> i) & 1;
        
        if (remainder >= d) {
            remainder -= d;
        }
    }
    
    return remainder;
}
