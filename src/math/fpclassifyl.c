/* This file has been put into the public domain by its author. */

#include <float.h>
#include <math.h>
#include <stdint.h>
#include <string.h>

int _Fpclassifyl(long double l)
{
#if LDBL_MANT_DIG == 53
    /* 64-bit long double (MSVC, ARM32) */
    return _Fpclassify((double)l);

#elif LDBL_MANT_DIG == 64
    /*. 80-bit long double (x86, m68k) */
    uint16_t sign_exp;
    uint64_t mantissa;

    static const uint32_t endian_check = 1;
    int is_little = (*(const uint8_t *)&endian_check == 1);

    if (is_little) {
        /* x86 layout: 8 byte mant, 2 byte sign/exp */
        memcpy(&mantissa, (const unsigned char *)&l, 8);
        memcpy(&sign_exp, (const unsigned char *)&l + 8, 2);
    } else {
        /* m68k layout: 2 byte sign/exp, 2 byte pad, 8 byte mant */
        memcpy(&sign_exp, (const unsigned char *)&l, 2);
        memcpy(&mantissa, (const unsigned char *)&l + 4, 8);
    }

    uint16_t exp = sign_exp & 0x7FFF;

    if (exp == 0) {
        return (mantissa == 0) ? FP_ZERO : FP_SUBNORMAL;
    }
    if (exp == 0x7FFF) {
        /* valid infinitys mant msb == 1, others 0 */
        return (mantissa == 0x8000000000000000ull) ? FP_INFINITE : FP_NAN;
    }
    return FP_NORMAL;

#elif LDBL_MANT_DIG == 113
    /* 128-bit IEEE 754 long double (AArch64, RISC-V) */
    uint64_t halves[2];
    static const uint32_t endian_check = 1;
    int is_little = (*(const uint8_t *)&endian_check == 1);

    memcpy(halves, &l, 16);

    uint64_t hi, lo;
    if (is_little) {
        hi = halves[1];
        lo = halves[0];
    } else {
        hi = halves[0];
        lo = halves[1];
    }

    /* 15 bit exp */
    uint32_t exp = (hi >> 48) & 0x7FFF;
    uint64_t frac_hi = hi & 0x0000FFFFFFFFFFFFull;
    uint64_t frac_lo = lo;

    if (exp == 0) {
        return (frac_hi == 0 && frac_lo == 0) ? FP_ZERO : FP_SUBNORMAL;
    }
    if (exp == 0x7FFF) {
        return (frac_hi == 0 && frac_lo == 0) ? FP_INFINITE : FP_NAN;
    }
    return FP_NORMAL;

#elif LDBL_MANT_DIG == 106
    /* IBM PowerPC double-double */
    return _Fpclassify((double)l);

#else
    /* fallback */
    if (l == 0.0L)
        return FP_ZERO;
    if (l != l)
        return FP_NAN;

    long double abs_l = (l < 0.0L) ? -l : l;
    if (abs_l * 0.5L == abs_l)
        return FP_INFINITE;
    if (abs_l < LDBL_MIN)
        return FP_SUBNORMAL;

    return FP_NORMAL;
#endif /* LDBL_MANT_DIG */
}
