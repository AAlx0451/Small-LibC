/* This file has been put into the public domain by its author. */

#include <errno.h>
#include <fenv.h>
#include <math.h>
#include <stdint.h>
#include <string.h>

/* Sollya Minimax Polynomial Coefficients for Sin/Cos */
static const double S0 = -0x1.5555555555555p-3;
static const double S1 = 0x1.111111111101fp-7;
static const double S2 = -0x1.a01a019fe8de3p-13;
static const double S3 = 0x1.71de3a10d6dabp-19;
static const double S4 = -0x1.ae63f8fdbd7f3p-26;
static const double S5 = 0x1.60e684a73f676p-33;
static const double S6 = -0x1.8db59ee9ebe87p-41;

static const double C0 = 0x1.5555555555555p-5;
static const double C1 = -0x1.6c16c16c16a34p-10;
static const double C2 = 0x1.a01a019fb851cp-16;
static const double C3 = -0x1.27e4faefd6ed7p-22;
static const double C4 = 0x1.1eecd77a908ep-29;
static const double C5 = -0x1.92a861320993ep-37;
static const double C6 = 0x1.35af7c91a696cp-45;

/* Constants for pi/2 reduction */
static const double InvPi2 = 0x1.45f306dc9c883p-1;    /* 2 / pi */
static const double PIO2_1 = 0x1.921fb54442d18p0;     /* pi/2 (first 53 bits) */
static const double PIO2_2 = 0x1.1a62633145c07p-54;   /* pi/2 (next 53 bits) */
static const double PIO2_3 = -0x1.f1976b7ed8fbcp-110; /* pi/2 (next 53 bits) */
static const double PIO2_4 = -0x1.4cf98e804177dp-165; /* pi/2 (tail) */

/* 2/pi bits in 32-bit chunks for Payne-Hanek reduction */
static const uint32_t TwoOverPi32[] = {
    0xA2F9836E, 0x4E441529, 0xFC2757D1, 0xF534DDC0, 0xDB629599, 0x3C439041, 0xFE5163AB, 0xDEBBC561,
    0xB7246E3A, 0x424DD2E0, 0x06492EEA, 0x09D1921C, 0xFE1DEB1C, 0xB129A73E, 0xE88235F5, 0x2EBB4484,
    0xE99C7026, 0xB45F7E41, 0x3991D639, 0x835339F4, 0x9C845F8B, 0xBDF9283B, 0x1FF897FF, 0xDE05980F,
    0xEF2F118B, 0x5A0A6D1F, 0x6D367ECF, 0x27CB09B7, 0x4F463F66, 0x9E5FEA2D, 0x7527BAC7, 0xEBE5F17C};

/*
 * Payne-Hanek style reduction for large arguments.
 * Uses 32-bit chunked multiplication and double-double arithmetic
 * to maintain precision for inputs with large exponents.
 */
static int rem_pio2_large(double x, double *r_hi, double *r_lo)
{
    uint64_t ix;
    memcpy(&ix, &x, 8);

    /* Extract exponent and mantissa from double-precision bit representation */
    int e = (int)((ix >> 52) & 0x7FF) - 1023;
    uint64_t m = (ix & 0xFFFFFFFFFFFFFULL) | 0x10000000000000ULL;

    /* Determine the starting index and bit-shift within the 2/pi table */
    int j = (e - 53) / 32;
    int shift = (e - 53) % 32;
    if (shift < 0) {
        j--;
        shift += 32;
    }

    /* Fetch 160 bits of 2/pi from the table */
    uint64_t w0 = (j >= 0 && j < 32) ? TwoOverPi32[j] : 0;
    uint64_t w1 = (j + 1 >= 0 && j + 1 < 32) ? TwoOverPi32[j + 1] : 0;
    uint64_t w2 = (j + 2 >= 0 && j + 2 < 32) ? TwoOverPi32[j + 2] : 0;
    uint64_t w3 = (j + 3 >= 0 && j + 3 < 32) ? TwoOverPi32[j + 3] : 0;
    uint64_t w4 = (j + 4 >= 0 && j + 4 < 32) ? TwoOverPi32[j + 4] : 0;

    /* Align fetched bits into four 32-bit blocks */
    uint32_t f0 = (uint32_t)(((w0 << shift) | (w1 >> (32 - shift))) & 0xFFFFFFFF);
    uint32_t f1 = (uint32_t)(((w1 << shift) | (w2 >> (32 - shift))) & 0xFFFFFFFF);
    uint32_t f2 = (uint32_t)(((w2 << shift) | (w3 >> (32 - shift))) & 0xFFFFFFFF);
    uint32_t f3 = (uint32_t)(((w3 << shift) | (w4 >> (32 - shift))) & 0xFFFFFFFF);

    /* Prepare mantissa and table blocks for long multiplication */
    uint32_t M[2] = {(uint32_t)(m & 0xFFFFFFFF), (uint32_t)(m >> 32)};
    uint32_t F[4] = {f3, f2, f1, f0};
    uint32_t R[6] = {0};

    /* Perform long multiplication (base 2^32): M * F */
    for (int i = 0; i < 2; i++) {
        uint64_t carry = 0;
        for (int k = 0; k < 4; k++) {
            uint64_t prod = (uint64_t)M[i] * F[k] + R[i + k] + carry;
            R[i + k] = (uint32_t)(prod & 0xFFFFFFFF);
            carry = prod >> 32;
        }
        R[i + 4] = (uint32_t)carry;
    }

    /* Determine the quadrant using the bits with weights 2^1 and 2^0 */
    unsigned int quad = ((R[4] & 1) << 1) | (R[3] >> 31);

    /* Pack 64 bits of the fractional part (starting from weight 2^-1) */
    uint64_t frac_bits =
        ((uint64_t)(R[3] & 0x7FFFFFFF) << 33) | ((uint64_t)R[2] << 1) | ((uint64_t)R[1] >> 31);

    /* Represent the fraction as a sum of two doubles to preserve full precision */
    double f_hi = (double)(frac_bits >> 32) * 0x1.0p-32;
    double f_lo = (double)(frac_bits & 0xFFFFFFFF) * 0x1.0p-64;

    /* Map range from [0, 1) to [-0.5, 0.5) and update quadrant */
    if (f_hi >= 0x1.0p-1) {
        f_hi -= 0x1.0p+0;
        quad++;
    }

    /* Multiply the precise fraction by pi/2 expansion using double-double arithmetic */
    double rhi = f_hi * PIO2_1;
    double err1 = fma(f_hi, PIO2_1, -rhi);

    double rlo = f_lo * PIO2_1 + f_hi * PIO2_2;

    /* Final normalization of the reduced argument */
    double rhi_final = rhi + rlo;
    double rlo_final = rlo - (rhi_final - rhi) + err1 + f_lo * PIO2_2 + f_hi * PIO2_3;

    *r_hi = rhi_final;
    *r_lo = rlo_final;

    return quad & 3;
}

double sin(double x)
{
    double abs_x = fabs(x);

    /* Special cases */

    /* If x is NaN, a NaN shall be returned. */
    if (isnan(x)) {
        return x;
    }

    /* Domain Error: The x argument is +/-Inf. */
    if (isinf(x)) {
        if (math_errhandling & MATH_ERRNO)
            errno = EDOM;
        if (math_errhandling & MATH_ERREXCEPT)
            feraiseexcept(FE_INVALID);
        return nan("");
    }

    /* Range Error: The value of x is subnormal. */
    if (fpclassify(x) == FP_SUBNORMAL) {
        if (math_errhandling & MATH_ERRNO)
            errno = ERANGE;
        if (math_errhandling & MATH_ERREXCEPT)
            feraiseexcept(FE_UNDERFLOW);
        return x;
    }

    /* If x is +/-0, x shall be returned. */
    if (x == 0x0.0p+0) {
        return x;
    }

    /* Handle very small arguments (~1e-5) to avoid underflow */
    if (abs_x < 0x1.4f8b588e368f1p-17) {
        /* Evaluated as x - (x * x * x) * (1/6) using the precise constant S0 */
        return x + (x * x * x) * S0;
    }

    double r_hi, r_lo;
    int quad;

    /*
     * Cody-Waite reduction for arguments up to ~1 million.
     * The threshold 2^20 ensures that kd doesn't obscure the precision of PIO2_1.
     */
    if (abs_x <= 0x1.0p+20) { /* 2^20 */
        /* Always compute using positive x to ensure logic symmetry */
        double y = abs_x * InvPi2;
        double kd = round(y);
        int64_t k = (int64_t)kd;
        quad = k & 3;

        /* First chunk is exact via Sterbenz Lemma */
        double kh = kd * PIO2_1;
        double kl = fma(kd, PIO2_1, -kh);
        
        double rh = abs_x - kh;
        double rl = -kl;

        /* Robust TwoSum algorithm for subtracting PIO2_2.
           Guarantees 100% accurate double-double error tracking
           regardless of relative operand magnitude. */
        double kh2 = kd * PIO2_2;
        double kl2 = fma(kd, PIO2_2, -kh2);
        
        double rh2 = rh - kh2;
        double a_virt2 = rh2 + kh2;
        double b_virt2 = rh2 - a_virt2;
        double rh2_err = (rh - a_virt2) - (kh2 + b_virt2);
        
        double rl2 = rl - kl2 + rh2_err;

        /* Robust TwoSum for subtracting PIO2_3 */
        double kh3 = kd * PIO2_3;
        double kl3 = fma(kd, PIO2_3, -kh3);
        
        double rh3 = rh2 - kh3;
        double a_virt3 = rh3 + kh3;
        double b_virt3 = rh3 - a_virt3;
        double rh3_err = (rh2 - a_virt3) - (kh3 + b_virt3);
        
        double rl3 = rl2 - kl3 + rh3_err;

        /* Subtract final minimal tail */
        rl3 -= kd * PIO2_4;

        /* Final highly accurate normalization */
        r_hi = rh3 + rl3;
        double a_virt_f = r_hi - rl3;
        double b_virt_f = r_hi - a_virt_f;
        r_lo = (rh3 - a_virt_f) + (rl3 - b_virt_f);

    } else {
        /* Payne-Hanek reduction for huge numbers */
        quad = rem_pio2_large(abs_x, &r_hi, &r_lo);
    }

    /* Apply symmetry rules for original negative input */
    if (x < 0x0.0p+0) {
        r_hi = -r_hi;
        r_lo = -r_lo;
        quad = (-quad) & 3;
    }

    /* Core function evaluations */
    double r2 = r_hi * r_hi;
    if ((quad & 1) == 0) {
        /* Core sine approximation: sin(r) ~ r + r^3 * P(r^2) */
        
        /* Compute r2_err to compensate precision in cubic calculations */
        double r2_err = fma(r_hi, r_hi, -r2);
        
        double p = S6;
        p = fma(p, r2, S5);
        p = fma(p, r2, S4);
        p = fma(p, r2, S3);
        p = fma(p, r2, S2);
        p = fma(p, r2, S1);
        p = fma(p, r2, S0);

        double r3 = r2 * r_hi;
        double poly = r3 * p;

        /*
         * Correctly compensate for the missing Taylor expansion cross term:
         * sin(hi + lo) approx sin(hi) + cos(hi)*lo approx sin(hi) + (1 - r2/2)*lo.
         */
        double r_lo_adj = fma(-0x1.0p-1 * r2, r_lo, r_lo);
        
        /* Add correction for r2_err in r3 evaluation.
           Missing term from poly is r_hi * r2_err * S0 */
        r_lo_adj += r_hi * r2_err * S0;

        /* Fast2Sum to add r_hi and poly flawlessly, preventing intermediate rounding loss */
        double s = r_hi + poly;
        double virt = s - r_hi;
        double poly_err = poly - virt;
        
        double res = s + (poly_err + r_lo_adj);

        return (quad == 0) ? res : -res;
    } else {
        /* Core cosine approximation: cos(r) ~ 1 - r^2/2 + r^4 * P(r^2) */
        double r2_err = fma(r_hi, r_hi, -r2);

        double p = C6;
        p = fma(p, r2, C5);
        p = fma(p, r2, C4);
        p = fma(p, r2, C3);
        p = fma(p, r2, C2);
        p = fma(p, r2, C1);
        p = fma(p, r2, C0);

        double r4 = r2 * r2;
        double poly = r4 * p;

        /*
         * Extremely accurate subtraction 1.0 - r_hi^2 / 2.
         * We compute `w = 1.0 - hz`, then use Fast2Sum to strictly 
         * recover the least significant bits of `hz` that were shifted out.
         */
        double hz = 0x1.0p-1 * r2;
        double w = 0x1.0p+0 - hz;
        double err = (0x1.0p+0 - w) - hz;

        /* Compensation term: r_hi * r_lo + 0.5 * r2_err */
        double comp = fma(r_hi, r_lo, 0x1.0p-1 * r2_err);

        /* Add Taylor series correction for sin(r_hi) * r_lo ≈ (r_hi + r_hi^3 * S0) * r_lo */
        double r3 = r_hi * r2;
        comp += r3 * r_lo * S0;

        /* Fast2Sum to combine w and poly perfectly without truncating poly bits */
        double s = w + poly;
        double virt = s - w;
        double poly_err = poly - virt;

        /* Evaluate final response rigorously: combine ultra-small errors together first */
        double res = s + (poly_err + (err - comp));

        return (quad == 1) ? res : -res;
    }
}
