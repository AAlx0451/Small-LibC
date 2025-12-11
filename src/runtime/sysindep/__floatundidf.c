#include <limits.h>

double __floatundidf(unsigned long long a) {
    if (a == 0) {
        return 0.0;
    }

    const unsigned long long sign_bit = 0;
    int msb_pos = 0;
    for (int i = (sizeof(a) * CHAR_BIT) - 1; i >= 0; i--) {
        if ((a >> i) & 1) {
            msb_pos = i;
            break;
        }
    }

    unsigned long long exponent = (unsigned long long)(msb_pos + 1023);
    a &= ~(1ULL << msb_pos);
    unsigned long long mantissa;
    if (msb_pos > 52) {
        mantissa = a >> (msb_pos - 52);
    } else {
        mantissa = a << (52 - msb_pos);
    }

    unsigned long long result_bits = (sign_bit << 63) | (exponent << 52) | mantissa;
    union {
        double d;
        unsigned long long u;
    } conv;
    conv.u = result_bits;
    return conv.d;
}
