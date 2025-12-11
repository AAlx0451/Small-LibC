#include <math.h>
#include <stdint.h>

double frexp(double value, int *exp) {
    union { double d; uint64_t u; } ieee;
    uint64_t exponent;

    ieee.d = value;

    if (value == 0.0) {
        *exp = 0;
        return value;
    }

    exponent = (ieee.u >> 52) & 0x7FF;

    if (exponent == 0x7FF) { 
        *exp = 0;
        return value; 
    }

    if (exponent == 0) {
        value *= 1.8014398509481984e16; /* 2^54 */
        ieee.d = value;
        exponent = (ieee.u >> 52) & 0x7FF;
        *exp = -54;
    } else {
        *exp = 0;
    }

    *exp += (int)exponent - 1022;
    ieee.u &= 0x800FFFFFFFFFFFFFULL;
    ieee.u |= 0x3FE0000000000000ULL; 

    return ieee.d;
}
