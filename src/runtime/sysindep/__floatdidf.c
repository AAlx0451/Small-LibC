#include <limits.h>

double __floatdidf(long long a) {
    if (a == 0) {
        return 0.0;
    }

    unsigned long long sign_bit = (a < 0);
    unsigned long long ua = (a < 0) ? -(unsigned long long)a : (unsigned long long)a;

    int msb_pos = 0;
    for (int i = 63; i >= 0; i--) {
        if ((ua >> i) & 1) {
            msb_pos = i;
            break;
        }
    }

    unsigned long long exponent = (unsigned long long)(msb_pos + 1023);
    
    ua &= ~(1ULL << msb_pos);

    unsigned long long mantissa;
    if (msb_pos > 52) {
        mantissa = ua >> (msb_pos - 52);
    } else {
        mantissa = ua << (52 - msb_pos);
    }

    unsigned long long result_bits = (sign_bit << 63) | (exponent << 52) | mantissa;

    union {
        double d;
        unsigned long long u;
    } conv;
    conv.u = result_bits;
    return conv.d;
}
