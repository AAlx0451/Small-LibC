/* This file has been put into the public domain by its author. */

#include <math.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <fenv.h>

static const uint32_t TwoOverPi32[] = {
    0xA2F9836E, 0x4E441529, 0xFC2757D1, 0xF534DDC0,
    0xDB629599, 0x3C439041, 0xFE5163AB, 0xDEBBC561,
    0xB7246E3A, 0x424DD2E0, 0x06492EEA, 0x09D1921C,
    0xFE1DEB1C, 0xB129A73E, 0xE88235F5, 0x2EBB4484,
    0xE99C7026, 0xB45F7E41, 0x3991D639, 0x835339F4,
    0x9C845F8B, 0xBDF9283B, 0x1FF897FF, 0xDE05980F,
    0xEF2F118B, 0x5A0A6D1F, 0x6D367ECF, 0x27CB09B7,
    0x4F463F66, 0x9E5FEA2D, 0x7527BAC7, 0xEBE5F17C
};

/* Full precision pi/2 chunks for Payne-Hanek */
static const double PIO2_1 = 0x1.921fb54442d18p+0;
static const double PIO2_2 = 0x1.1a62633145c07p-54;
static const double PIO2_3 = 0x1.b839a252049c1p-104;

/* Cody-Waite pi/2 chunks for fast reduction (CW_PIO2_1 has 21 trailing bits of zero) */
static const double CW_PIO2_1 = 0x1.921fb54400000p+0;
static const double CW_PIO2_2 = 0x1.0b4611a626331p-34;
static const double CW_PIO2_3 = 0x1.3198a2e037073p-69;

static const double INV_PIO2  = 0x1.45f306dc9c883p-1; /* 2/pi */

/* Polynomial coefficients for tan(r) = r + r^3 * Q(r^2) */
static const double C0  =  0x1.5555555555555p-2;
static const double C1  =  0x1.111111111114bp-3;
static const double C2  =  0x1.ba1ba1ba1eae3p-5;
static const double C3  =  0x1.664f488018e08p-6;
static const double C4  =  0x1.226e360ae16c5p-7;
static const double C5  =  0x1.d6d3a2f58150ap-9;
static const double C6  =  0x1.7da7282c5eb51p-10;
static const double C7  =  0x1.3523ae40610b1p-11;
static const double C8  =  0x1.f96fe3d4aac8bp-13;
static const double C9  =  0x1.7bb3294d0a2f4p-14;
static const double C10 =  0x1.ca2c40b9d2b1bp-15;
static const double C11 = -0x1.4946197edc81ep-17;
static const double C12 =  0x1.2a1abbf8dd9fep-15;
static const double C13 = -0x1.2b568f1bb38fbp-16;
static const double C14 =  0x1.1935262e16c74p-17;

static int rem_pio2_large(double x, double *r_hi, double *r_lo)
{
    uint64_t ix, m;
    int e, j, shift, i, k;
    unsigned int quad;
    uint64_t w0, w1, w2, w3, w4, frac_bits;
    uint32_t f0, f1, f2, f3;
    uint32_t M[2], F[4], R[6];
    double f_hi, f_lo, rhi, err1, rlo, rhi_final, rlo_final;

    memcpy(&ix, &x, 8);
    e = (int)((ix >> 52) & 0x7FF) - 1023;
    m = (ix & 0xFFFFFFFFFFFFFULL) | 0x10000000000000ULL;

    j = (e - 53) / 32;
    shift = (e - 53) % 32;
    if (shift < 0) {
        j--;
        shift += 32;
    }

    w0 = (j >= 0 && j < 32) ? TwoOverPi32[j] : 0;
    w1 = (j + 1 >= 0 && j + 1 < 32) ? TwoOverPi32[j + 1] : 0;
    w2 = (j + 2 >= 0 && j + 2 < 32) ? TwoOverPi32[j + 2] : 0;
    w3 = (j + 3 >= 0 && j + 3 < 32) ? TwoOverPi32[j + 3] : 0;
    w4 = (j + 4 >= 0 && j + 4 < 32) ? TwoOverPi32[j + 4] : 0;

    f0 = (uint32_t)(((w0 << shift) | (w1 >> (32 - shift))) & 0xFFFFFFFF);
    f1 = (uint32_t)(((w1 << shift) | (w2 >> (32 - shift))) & 0xFFFFFFFF);
    f2 = (uint32_t)(((w2 << shift) | (w3 >> (32 - shift))) & 0xFFFFFFFF);
    f3 = (uint32_t)(((w3 << shift) | (w4 >> (32 - shift))) & 0xFFFFFFFF);

    M[0] = (uint32_t)(m & 0xFFFFFFFF);
    M[1] = (uint32_t)(m >> 32);
    F[0] = f3; F[1] = f2; F[2] = f1; F[3] = f0;
    
    memset(R, 0, sizeof(R));

    for (i = 0; i < 2; i++) {
        uint64_t carry = 0;
        for (k = 0; k < 4; k++) {
            uint64_t prod = (uint64_t)M[i] * F[k] + R[i + k] + carry;
            R[i + k] = (uint32_t)(prod & 0xFFFFFFFF);
            carry = prod >> 32;
        }
        R[i + 4] = (uint32_t)carry;
    }

    quad = ((R[4] & 1) << 1) | (R[3] >> 31);
    frac_bits = ((uint64_t)(R[3] & 0x7FFFFFFF) << 33) 
              | ((uint64_t)R[2] << 1) 
              | ((uint64_t)R[1] >> 31);

    f_hi = (double)(frac_bits >> 32) * 0x1.0p-32;
    f_lo = (double)(frac_bits & 0xFFFFFFFF) * 0x1.0p-64;

    if (f_hi >= 0x1.0p-1) {
        f_hi -= 0x1.0p+0; 
        quad++;
    }

    rhi = f_hi * PIO2_1;
    err1 = fma(f_hi, PIO2_1, -rhi); 
    rlo = f_lo * PIO2_1 + f_hi * PIO2_2;

    rhi_final = rhi + rlo;
    rlo_final = rlo - (rhi_final - rhi) + err1 + f_lo * PIO2_2 + f_hi * PIO2_3;

    *r_hi = rhi_final;
    *r_lo = rlo_final;

    return quad & 3;
}

double tan(double x)
{
    double ax = fabs(x);

    /* --- Special cases and Error Handling --- */

    /* If x is NaN, a NaN shall be returned. */
    if (isnan(x)) {
        return x;
    }

    /* Domain Error: The x argument is +/-Inf. */
    if (isinf(x)) {
        if (math_errhandling & MATH_ERRNO) errno = EDOM;
        if (math_errhandling & MATH_ERREXCEPT) feraiseexcept(FE_INVALID);
        return nan("");
    }

    /* Range Error: The value of x is subnormal. */
    if (fpclassify(x) == FP_SUBNORMAL) {
        if (math_errhandling & MATH_ERRNO) errno = ERANGE;
        if (math_errhandling & MATH_ERREXCEPT) feraiseexcept(FE_UNDERFLOW);
        return x;
    }

    /* If x is +/-0, x shall be returned. */
    if (x == 0x0.0p+0) {
        return x;
    }

    /* --- Main Logic --- */

    int quad = 0;
    double r_hi, r_lo, qd, r_hi_new;
    double t_hi, t_err, t_lo, P, u_hi, u_lo, q_hi, q_lo;
    double s_hi, s_err2, s_lo, w_hi, w_err, w_lo;
    double tan_hi, tan_v, tan_lo, d_hi, d_lo, inv_hi, inv_err, inv_lo;

    if (ax <= 0x1.921fb54442d18p-1) { /* 0.7853981633974483 */
        r_hi = ax;
        r_lo = 0x0.0p+0;
    } else if (ax < 0x1.0p+20) {      /* 1048576.0 */
        qd = round(ax * INV_PIO2);
        quad = (int)qd;
        
        /* CW_PIO2_1 has 21 trailing zeros, meaning ax - qd*CW_PIO2_1 is exact for qd < 2^21 */
        r_hi = ax - qd * CW_PIO2_1;
        r_lo = -qd * CW_PIO2_2 - qd * CW_PIO2_3;
        
        r_hi_new = r_hi + r_lo;
        r_lo = r_lo - (r_hi_new - r_hi);
        r_hi = r_hi_new;
    } else {
        quad = rem_pio2_large(ax, &r_hi, &r_lo);
    }

    if (x < 0x0.0p+0) {
        r_hi = -r_hi;
        r_lo = -r_lo;
        quad = -quad;
    }

    t_hi = r_hi * r_hi;
    t_err = fma(r_hi, r_hi, -t_hi);
    t_lo = t_err + 0x1.0p+1 * r_hi * r_lo; /* 2.0 * r_hi * r_lo */

    /* Evaluate degree 14 polynomial via unrolled Horner scheme */
    P = fma(C14, t_hi, C13);
    P = fma(P, t_hi, C12);
    P = fma(P, t_hi, C11);
    P = fma(P, t_hi, C10);
    P = fma(P, t_hi, C9);
    P = fma(P, t_hi, C8);
    P = fma(P, t_hi, C7);
    P = fma(P, t_hi, C6);
    P = fma(P, t_hi, C5);
    P = fma(P, t_hi, C4);
    P = fma(P, t_hi, C3);
    P = fma(P, t_hi, C2);
    P = fma(P, t_hi, C1);

    /* Double-double math to reconstruct precise response */
    u_hi = t_hi * P;
    u_lo = fma(t_hi, P, -u_hi) + t_lo * P;

    q_hi = C0 + u_hi;
    q_lo = (u_hi - (q_hi - C0)) + u_lo;

    s_hi = t_hi * q_hi;
    s_err2 = fma(t_hi, q_hi, -s_hi);
    s_lo = s_err2 + t_hi * q_lo + t_lo * q_hi;

    w_hi = r_hi * s_hi;
    w_err = fma(r_hi, s_hi, -w_hi);
    w_lo = w_err + r_hi * s_lo + r_lo * s_hi;

    tan_hi = r_hi + w_hi;
    tan_v = tan_hi - r_hi;
    tan_lo = (r_hi - (tan_hi - tan_v)) + (w_hi - tan_v) + r_lo + w_lo;

    if ((quad & 1) == 0) {
        return tan_hi + tan_lo;
    } else {
        /* Return -1 / tan(r) with full precision division via Newton-Raphson correction */
        d_hi = tan_hi;
        d_lo = tan_lo;
        inv_hi = -0x1.0p+0 / d_hi;
        inv_err = fma(d_hi, -inv_hi, -0x1.0p+0);
        inv_lo = (inv_err - d_lo * inv_hi) / d_hi;
        return inv_hi + inv_lo;
    }
}
