#include "runtime.h"
#include <string.h>

// Portable Count Leading Zeros
static int clz64(unsigned long long x) {
    int n = 0;
    if(x == 0)
        return 64;
    if(!(x & 0xFFFFFFFF00000000ULL)) {
        n += 32;
        x <<= 32;
    }
    if(!(x & 0xFFFF000000000000ULL)) {
        n += 16;
        x <<= 16;
    }
    if(!(x & 0xFF00000000000000ULL)) {
        n += 8;
        x <<= 8;
    }
    if(!(x & 0xF000000000000000ULL)) {
        n += 4;
        x <<= 4;
    }
    if(!(x & 0xC000000000000000ULL)) {
        n += 2;
        x <<= 2;
    }
    if(!(x & 0x8000000000000000ULL)) {
        n += 1;
    }
    return n;
}

double __floatdidf(long long a) {
    if(a == 0)
        return 0.0;

    unsigned long long sign_bit = (a < 0) ? 1 : 0;
    unsigned long long ua = (a < 0) ? -(unsigned long long)a : (unsigned long long)a;

    int shift = clz64(ua);
    int exponent = 1023 + (63 - shift);

    unsigned long long mantissa;
    int shift_diff = 63 - shift - 52;

    if(shift_diff > 0) {
        // Round to Nearest, ties to Even
        unsigned long long round_mask = (1ULL << (shift_diff - 1));
        unsigned long long sticky_mask = round_mask - 1;

        unsigned long long guard = (ua >> (shift_diff - 1)) & 1;
        unsigned long long sticky = (ua & sticky_mask) ? 1 : 0;
        unsigned long long lsb = (ua >> shift_diff) & 1;

        mantissa = ua >> shift_diff;
        if(guard && (sticky || lsb)) {
            mantissa++;
            if(mantissa & (1ULL << 53)) { // Handle overflow from rounding
                mantissa >>= 1;
                exponent++;
            }
        }
    } else {
        mantissa = ua << (-shift_diff);
    }

    mantissa &= 0x000FFFFFFFFFFFFFULL; // Clear implicit 1
    unsigned long long result_bits = (sign_bit << 63) | ((unsigned long long)exponent << 52) | mantissa;

    double result;
    memcpy(&result, &result_bits, sizeof(result));
    return result;
}
