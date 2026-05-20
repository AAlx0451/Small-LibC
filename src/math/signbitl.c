/* This file has been put into the public domain by its author. */

#include <float.h>
#include <math.h>
#include <stdint.h>
#include <string.h>

int _Signbitl(long double l)
{
#if LDBL_MANT_DIG == 53
    /* 64-bit long double (MSVC, ARM) */
    return _Signbit((double)l);

#elif LDBL_MANT_DIG == 64
    /* 80-bit long double (x86, m68k) */
    static const uint16_t endian_check = 1;
    int is_little = (*(const uint8_t *)&endian_check == 1);

    uint16_t sign_exp;
    if (is_little) {
        /* x86: last 2 bytes are sign */
        memcpy(&sign_exp, (const unsigned char *)&l + 8, 2);
    } else {
        /* m68k: first 2 bytes are sign */
        memcpy(&sign_exp, (const unsigned char *)&l, 2);
    }
    /* sing is 15 byte at 16byte part */
    return (sign_exp >> 15) & 1;

#elif LDBL_MANT_DIG == 113
    /* 128-bit long double (AArch64, RISC-V) */
    static const uint16_t endian_check = 1;
    int is_little = (*(const uint8_t *)&endian_check == 1);

    uint64_t halves[2];
    memcpy(halves, &l, 16);

    /* msbs only */
    uint64_t hi = is_little ? halves[1] : halves[0];
    return (hi >> 63) & 1;

#elif LDBL_MANT_DIG == 106
    /* IBM double-double */
    return _Signbit((double)l);

#else
    /* fallback */
    return _Signbit((double)l);
#endif /* LDBL_MANT_DIG */
}
