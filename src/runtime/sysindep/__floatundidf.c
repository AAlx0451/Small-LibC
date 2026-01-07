#include <string.h>

static int clz64(unsigned long long x) {
    int n = 0;
    if (x == 0) return 64;
    if (!(x & 0xFFFFFFFF00000000ULL)) { n += 32; x <<= 32; }
    if (!(x & 0xFFFF000000000000ULL)) { n += 16; x <<= 16; }
    if (!(x & 0xFF00000000000000ULL)) { n += 8;  x <<= 8; }
    if (!(x & 0xF000000000000000ULL)) { n += 4;  x <<= 4; }
    if (!(x & 0xC000000000000000ULL)) { n += 2;  x <<= 2; }
    if (!(x & 0x8000000000000000ULL)) { n += 1; }
    return n;
}

double __floatundidf(unsigned long long a) {
    if (a == 0) return 0.0;

    int shift = clz64(a);
    int exponent = 1023 + (63 - shift);
    
    unsigned long long mantissa;
    int shift_diff = 63 - shift - 52; 

    if (shift_diff > 0) {
        unsigned long long round_mask = (1ULL << (shift_diff - 1));
        unsigned long long sticky_mask = round_mask - 1;
        
        unsigned long long guard = (a >> (shift_diff - 1)) & 1; 
        unsigned long long sticky = (a & sticky_mask) ? 1 : 0; 
        unsigned long long lsb = (a >> shift_diff) & 1;

        mantissa = a >> shift_diff;
        if (guard && (sticky || lsb)) {
            mantissa++;
            if (mantissa & (1ULL << 53)) {
                mantissa >>= 1;
                exponent++;
            }
        }
    } else {
        mantissa = a << (-shift_diff);
    }

    mantissa &= 0x000FFFFFFFFFFFFFULL;
    unsigned long long result_bits = ((unsigned long long)exponent << 52) | mantissa;

    double result;
    memcpy(&result, &result_bits, sizeof(result));
    return result;
}
