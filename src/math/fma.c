/* This file has been put into the public domain by its author. */

#include <errno.h>
#include <fenv.h>
#include <math.h>
#include <stdint.h>
#include <string.h>

#if defined(__clang__) || defined(__GNUC__)
# pragma GCC diagnostic ignored "-Wunknown-pragmas"
#endif /* __clang__ || __GNUC__ */

#pragma STDC FENV_ACCESS ON

/*
 * a = (ah * 2^32 + al)
 * b = (bh * 2^32 + bl)
 * a * b = ah*bh*2^64 + (ah*bl + al*bh)*2^32 + al*bl
 */
static inline void mul_64x64(uint64_t a, uint64_t b, uint64_t *hi, uint64_t *lo)
{
    uint32_t ah = a >> 32, al = (uint32_t)a;
    uint32_t bh = b >> 32, bl = (uint32_t)b;

    uint64_t ll = (uint64_t)al * bl;
    uint64_t hl = (uint64_t)ah * bl;
    uint64_t lh = (uint64_t)al * bh;
    uint64_t hh = (uint64_t)ah * bh;

    uint64_t cross = (ll >> 32) + (uint32_t)hl + (uint32_t)lh;
    *lo = (cross << 32) | (uint32_t)ll;
    *hi = hh + (hl >> 32) + (lh >> 32) + (cross >> 32);
}

/* 128-bit left shift */
static inline void shl_128(uint64_t *hi, uint64_t *lo, int s)
{
    if (s == 0)
        return;
    if (s >= 128) {
        *hi = 0;
        *lo = 0;
    } else if (s >= 64) {
        *hi = *lo << (s - 64);
        *lo = 0;
    } else {
        *hi = (*hi << s) | (*lo >> (64 - s));
        *lo <<= s;
    }
}

/* 128-bit right shift. bits shifted out are accumulated into the sticky bit. */
static inline void shr_128_sticky(uint64_t *hi, uint64_t *lo, uint32_t *sticky, int s)
{
    if (s == 0)
        return;
    if (s >= 128) {
        if (*hi || *lo)
            *sticky = 1;
        *hi = *lo = 0;
    } else if (s >= 64) {
        int shift = s - 64;
        uint64_t lost_hi = shift ? (*hi & ((1ULL << shift) - 1)) : 0;
        if (*lo || lost_hi)
            *sticky = 1;
        *lo = *hi >> shift;
        *hi = 0;
    } else {
        if (*lo & ((1ULL << s) - 1))
            *sticky = 1;
        uint64_t new_lo = (*lo >> s) | (*hi << (64 - s));
        *hi >>= s;
        *lo = new_lo;
    }
}

/* count leading zeros */
static inline int clz64(uint64_t x)
{
    if (x == 0)
        return 64;
    int n = 0;
    if (!(x >> 32)) {
        n += 32;
        x <<= 32;
    }
    if (!(x >> 48)) {
        n += 16;
        x <<= 16;
    }
    if (!(x >> 56)) {
        n += 8;
        x <<= 8;
    }
    if (!(x >> 60)) {
        n += 4;
        x <<= 4;
    }
    if (!(x >> 62)) {
        n += 2;
        x <<= 2;
    }
    if (!(x >> 63)) {
        n += 1;
    }
    return n;
}

static inline int highest_bit_128(uint64_t hi, uint64_t lo)
{
    if (hi)
        return 127 - clz64(hi);
    if (lo)
        return 63 - clz64(lo);
    return -1;
}

/* unpack double into sign, unbiased exponent, and mantissa */
static inline void unpack(double d, uint64_t *man, int *exp, int *sign)
{
    uint64_t u;
    memcpy(&u, &d, sizeof(u));
    *sign = u >> 63;
    *exp = (u >> 52) & 0x7FF;
    *man = u & 0xFFFFFFFFFFFFF;

    /* handle subnormal numbers */
    if (*exp == 0) {
        if (*man == 0) {
            *exp = -1023;
            return;
        }
        *exp = 1;
        while ((*man & (1ULL << 52)) == 0) {
            *man <<= 1;
            (*exp)--;
        }
    } else {
        *man |= (1ULL << 52); /* restore implicit bit */
    }
    *exp -= 1023;
}

double fma(double x, double y, double z)
{
    int cx = fpclassify(x);
    int cy = fpclassify(y);
    int cz = fpclassify(z);

    /* propagate NaNs and properly raise FE_INVALID on sNaN via hardware + */
    if (cx == FP_NAN || cy == FP_NAN || cz == FP_NAN) {
        return x + y + z;
    }

    /* Inf * 0 is NaN */
    if ((cx == FP_INFINITE && cy == FP_ZERO) || (cx == FP_ZERO && cy == FP_INFINITE)) {
        if (math_errhandling & MATH_ERRNO)
            errno = EDOM;
        if (math_errhandling & MATH_ERREXCEPT)
            feraiseexcept(FE_INVALID);
        return nan("");
    }

    /* handle infinities */
    if (cx == FP_INFINITE || cy == FP_INFINITE) {
        int sign_xy = signbit(x) ^ signbit(y);
        /* Inf - Inf is NaN */
        if (cz == FP_INFINITE && signbit(z) != sign_xy) {
            if (math_errhandling & MATH_ERRNO)
                errno = EDOM;
            if (math_errhandling & MATH_ERREXCEPT)
                feraiseexcept(FE_INVALID);
            return nan("");
        }
        return sign_xy ? -HUGE_VAL : HUGE_VAL;
    }
    if (cz == FP_INFINITE)
        return z;

    /* delegate exact hardware handling of 0 signs */
    if (cx == FP_ZERO || cy == FP_ZERO)
        return (x * y) + z;

    int sx, sy, sz, ex, ey, ez;
    uint64_t mx, my, mz;
    unpack(x, &mx, &ex, &sx);
    unpack(y, &my, &ey, &sy);
    unpack(z, &mz, &ez, &sz);

    /* exact product of mantissas */
    uint64_t phi, plo;
    mul_64x64(mx, my, &phi, &plo);
    int ep = ex + ey;
    int sp = sx ^ sy;

    /*
     * magic shift: 16-bit guard window.
     * to perform an exact fma without catastrophic cancellation loss,
     * we shift the product left by 16 bits before alignment.
     * if ep ~= ez and signs differ, the lower bits will not be lost
     * into the sticky bit.
     * mathematically, 16 bits > 11 bits (max cancellation difference).
     */
    shl_128(&phi, &plo, 16);

    /* normalize to [1;2) */
    if (highest_bit_128(phi, plo) == 121) {
        uint64_t new_lo = (plo >> 1) | (phi << 63);
        phi >>= 1;
        plo = new_lo;
        ep++;
    }

    uint64_t zhi = 0, zlo = mz;

    /*
     * align z implicit bit with the shifted product.
     * originally at position 52, shifted by 52 (to 104) + 16 (guard) = 68.
     */
    shl_128(&zhi, &zlo, 68);

    /* map zero to a tiny exponent to avoid alignment issues */
    if (mz == 0)
        ez = ep - 1000;

    /* determine magnitude to always subtract smaller from larger */
    int cmp;
    if (ep > ez)
        cmp = 1;
    else if (ep < ez)
        cmp = -1;
    else {
        if (phi > zhi)
            cmp = 1;
        else if (phi < zhi)
            cmp = -1;
        else {
            if (plo > zlo)
                cmp = 1;
            else if (plo < zlo)
                cmp = -1;
            else
                cmp = 0;
        }
    }

    /* exact cancellation yields +0.0 (or -0.0 in downward rounding mode) */
    if (cmp == 0 && sp != sz) {
        return (fegetround() == FE_DOWNWARD) ? -0.0 : 0.0;
    }

    uint64_t ahi, alo, bhi, blo;
    uint32_t sticky = 0;
    int r_sign, e_base;

    if (cmp >= 0) {
        ahi = phi;
        alo = plo;
        bhi = zhi;
        blo = zlo;
        r_sign = sp;
        e_base = ep;
        shr_128_sticky(&bhi, &blo, &sticky, ep - ez);
    } else {
        ahi = zhi;
        alo = zlo;
        bhi = phi;
        blo = plo;
        r_sign = sz;
        e_base = ez;
        shr_128_sticky(&bhi, &blo, &sticky, ez - ep);
    }

    /* arithmetic operation */
    uint64_t rhi, rlo;
    uint32_t r_sticky = sticky;

    if (sp == sz) {
        rlo = alo + blo;
        rhi = ahi + bhi + (rlo < alo);
    } else {
        uint64_t diff_lo = alo - blo;
        uint64_t borrow = (alo < blo) ? 1 : 0;
        if (sticky) {
            if (diff_lo == 0)
                borrow++;
            diff_lo--;
        }
        rlo = diff_lo;
        rhi = ahi - bhi - borrow;
    }

    if (rhi == 0 && rlo == 0) {
        return (fegetround() == FE_DOWNWARD) ? -0.0 : 0.0;
    }

    /* normalize result: shift implicit bit back to 104 (120 - 16) */
    int hb = highest_bit_128(rhi, rlo);
    e_base += (hb - 120); /* normalize exp after 16 bit shift */

    if (hb > 104) {
        int shift = hb - 104;
        shr_128_sticky(&rhi, &rlo, &r_sticky, shift);
    } else if (hb < 104) {
        int shift = 104 - hb;
        shl_128(&rhi, &rlo, shift);
    }

    /* ieee 754 float64 bias is 1023 */
    int target_exp = e_base + 1023;
    int is_tiny = (target_exp <= 0);

    /* handle underflow to subnormals */
    if (target_exp <= 0) {
        shr_128_sticky(&rhi, &rlo, &r_sticky, 1 - target_exp);
        target_exp = 0;
    }

    int round_bit = (rlo >> 51) & 1;
    int sticky_bits = (rlo & ((1ULL << 51) - 1)) || r_sticky;
    int lsb = (rlo >> 52) & 1;
    int is_inexact = round_bit || sticky_bits;

    /* raise standard fenv exceptions */
    if (is_inexact) {
        feraiseexcept(FE_INEXACT);
        if (is_tiny) {
            if (math_errhandling & MATH_ERRNO)
                errno = ERANGE;
            if (math_errhandling & MATH_ERREXCEPT)
                feraiseexcept(FE_UNDERFLOW);
        }
    }

    /* apply dynamic rounding modes */
    int rmode = fegetround();
    int round_up = 0;
    if (rmode == FE_TONEAREST) {
        round_up = round_bit && (sticky_bits || lsb);
    } else if (rmode == FE_UPWARD) {
        round_up = is_inexact && !r_sign;
    } else if (rmode == FE_DOWNWARD) {
        round_up = is_inexact && r_sign;
    } /* FE_TOWARDZERO needs no round up */

    uint64_t final_man = (rlo >> 52) | (rhi << 12);

    if (round_up) {
        final_man++;
        /* mantissa overflow during rounding */
        if (final_man & (1ULL << 53)) {
            final_man >>= 1;
            target_exp++;
        }
    }

    /* pack ieee 754 bits */
    if (target_exp > 0) {
        final_man &= ~(1ULL << 52);
    } else {
        if (final_man & (1ULL << 52)) {
            target_exp = 1;
            final_man &= ~(1ULL << 52);
        }
    }

    /* check for overflow */
    if (target_exp >= 2047) {
        if (math_errhandling & MATH_ERRNO)
            errno = ERANGE;
        if (math_errhandling & MATH_ERREXCEPT)
            feraiseexcept(FE_OVERFLOW | FE_INEXACT);

        /* check rounding mode for overflow result */
        int is_inf = 1;
        if (rmode == FE_TOWARDZERO)
            is_inf = 0;
        else if (rmode == FE_DOWNWARD && r_sign == 0)
            is_inf = 0;
        else if (rmode == FE_UPWARD && r_sign == 1)
            is_inf = 0;

        uint64_t final_inf_or_max = is_inf ? (0x7FFULL << 52) : 0x7FEFFFFFFFFFFFFFULL;
        uint64_t overflow_result = ((uint64_t)r_sign << 63) | final_inf_or_max;

        double ret;
        memcpy(&ret, &overflow_result, sizeof(ret));
        return ret;
    }

    uint64_t result = ((uint64_t)r_sign << 63) | ((uint64_t)target_exp << 52) | final_man;
    double final_d;
    memcpy(&final_d, &result, sizeof(final_d));

    return final_d;
}
