#include <limits.h>

unsigned int __udivsi3(unsigned int n, unsigned int d) {
    if (d == 0) return 0;
    
    unsigned int quotient = 0;
    unsigned int remainder = 0;

    for (int i = 31; i >= 0; i--) {
        remainder = (remainder << 1) | ((n >> i) & 1);
        if (remainder >= d) {
            remainder -= d;
            quotient |= (1U << i);
        }
    }
    return quotient;
}
