/* This file has been put into the public domain by its author. */

#include <errno.h>
#include <fenv.h>
#include <math.h>

/* double-double arithmetics. */
#define LN2_HI  0x1.62e42fefa3800p-1
#define LN2_LO  0x1.ef35793c76730p-45

/* sqrt(1.0/2) */
#define SQRT1_2 0x1.6a09e667f3bcdp-1

double log(double x) {
    /* special cases */
    if (isnan(x)) return x;

    if (x == 0.0) {
        if (math_errhandling & MATH_ERRNO) errno = ERANGE;
        if (math_errhandling & MATH_ERREXCEPT) feraiseexcept(FE_DIVBYZERO);
        return -INFINITY;
    }

    if (x < 0.0) {
        if (math_errhandling & MATH_ERRNO) errno = EDOM;
        if (math_errhandling & MATH_ERREXCEPT) feraiseexcept(FE_INVALID);
        return NAN;
    }

    if (isinf(x)) return x;
    if (x == 1.0) return 0.0;

    int e;
    /* x = m * 2^e, m in [0.5, 1.0) */
    double m = frexp(x, &e);

    /* x = m * 2^e, m in [sqrt(2)/2, sqrt(2)] */
    if (m < SQRT1_2) {
        m *= 2.0;
        e -= 1;
    }

    /* z is in [sqrt(2)/2 - 1, sqrt(2) - 1] */
    double z = m - 1.0;

    /* polynomial is: z * (1 + z * (-0.5 + z * Q(z))) */
    double q;
    q = -0x1.910632e47f27cp-6;
    q = fma(z, q,  0x1.d54bfab6cdc0cp-5);
    q = fma(z, q, -0x1.016cd661e75f7p-4);
    q = fma(z, q,  0x1.baec35a655cb6p-5);
    q = fma(z, q, -0x1.b3e3f78bf942p-5);
    q = fma(z, q,  0x1.dca53161db1f4p-5);
    q = fma(z, q, -0x1.00db26d11e916p-4);
    q = fma(z, q,  0x1.116abe4d83bf5p-4);
    q = fma(z, q, -0x1.24882f844b96dp-4);
    q = fma(z, q,  0x1.3b0d4f9209f9cp-4);
    q = fma(z, q, -0x1.55558695863ddp-4);
    q = fma(z, q,  0x1.745d5be9fe2f5p-4);
    q = fma(z, q, -0x1.99999a1aee80cp-4);
    q = fma(z, q,  0x1.c71c700eec746p-4);
    q = fma(z, q, -0x1.fffffff62aa32p-4);
    q = fma(z, q,  0x1.24924927b2929p-3);
    q = fma(z, q, -0x1.5555555567ff2p-3);
    q = fma(z, q,  0x1.9999999993f5p-3);
    q = fma(z, q, -0x1.ffffffffffeabp-3);
    q = fma(z, q,  0x1.5555555555572p-2);
    q = fma(z, q, -0x1.0000000000000p-1);

    /* log(x) = e*ln(2) + log(1+z)
       log(1+z) ~ z + z^2 * q
       We use double-double for e*ln(2) and group 'z' with LN2_HI for precision. */
    
    double ee = (double)e;
    
    /* tail = e*LN2_LO + z^2 * q */
    double tail = fma(z * z, q, ee * LN2_LO);
    
    /* we add the smallest terms first. */
    double res = z + tail;
    res += ee * LN2_HI;

    return res;
}
