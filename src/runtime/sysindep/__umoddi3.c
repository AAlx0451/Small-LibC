#include "runtime.h"
#include <limits.h>

unsigned long long __umoddi3(unsigned long long n, unsigned long long d) {
    int i;
    unsigned long long remainder = 0;

    if(d == 0)
        return 0;

    for(i = 63; i >= 0; i--) {
        remainder = (remainder << 1) | ((n >> i) & 1);
        if(remainder >= d) {
            remainder -= d;
        }
    }
    return remainder;
}
