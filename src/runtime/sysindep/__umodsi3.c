#include "runtime.h"
#include <limits.h>

unsigned int __umodsi3(unsigned int n, unsigned int d) {
    if(d == 0)
        return 0;

    unsigned int remainder = 0;

    for(int i = 31; i >= 0; i--) {
        remainder = (remainder << 1) | ((n >> i) & 1);
        if(remainder >= d) {
            remainder -= d;
        }
    }
    return remainder;
}
