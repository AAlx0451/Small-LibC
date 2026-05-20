/* This file has been put into the public domain by its author. */

#include <errno.h>
#include <fenv.h>
#include <float.h>
#include <math.h>
#include <stdint.h>
#include <string.h>

double ldexp(double num, int ex)
{
    uint64_t bits;
    int64_t e;

    switch (fpclassify(num)) {
    case FP_NAN:
    case FP_INFINITE:
    case FP_ZERO:
        return num;
    case FP_SUBNORMAL:
        while (fpclassify(num) == FP_SUBNORMAL) {
            num *= 1073741824.0; /* 2^30 */
            ex -= 30;
        }
        break;
    }

    memcpy(&bits, &num, sizeof(double));
    e = (int64_t)((bits >> 52) & 0x7FF) + ex;

    if (e >= 2047) { /* overflow */
        if (math_errhandling & MATH_ERRNO)
            errno = ERANGE;
        if (math_errhandling & MATH_ERREXCEPT)
            feraiseexcept(FE_OVERFLOW | FE_INEXACT);
        switch (fegetround()) {
        case FE_TOWARDZERO:
            return (bits & 0x8000000000000000ULL) ? -DBL_MAX : DBL_MAX;
        case FE_UPWARD:
            return (bits & 0x8000000000000000ULL) ? -DBL_MAX : HUGE_VAL;
        case FE_DOWNWARD:
            return (bits & 0x8000000000000000ULL) ? -HUGE_VAL : DBL_MAX;
        default:
            return (bits & 0x8000000000000000ULL) ? -HUGE_VAL : HUGE_VAL;
        }
    }

    if (e <= 0) {      /* potential underflow */
        if (e < -54) { /* too small even for subnormal */
            if (math_errhandling & MATH_ERRNO)
                errno = ERANGE;
            if (math_errhandling & MATH_ERREXCEPT)
                feraiseexcept(FE_UNDERFLOW | FE_INEXACT);
            return (bits & 0x8000000000000000ULL) ? -0.0 : 0.0;
        }

        bits = (bits & 0x800FFFFFFFFFFFFFULL) | (1ULL << 52);
        memcpy(&num, &bits, sizeof(double));

        double scale;
        uint64_t s_bits = (uint64_t)(1023 + (e - 1)) << 52;
        memcpy(&scale, &s_bits, sizeof(double));

        double res = num * scale;
        if (fpclassify(res) == FP_ZERO)
            if (math_errhandling & MATH_ERRNO)
                errno = ERANGE;
        return res;
    }

    /* normal result */
    bits = (bits & 0x800FFFFFFFFFFFFFULL) | ((uint64_t)e << 52);
    memcpy(&num, &bits, sizeof(double));
    return num;
}
