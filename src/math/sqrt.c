/* This file has been put into the public domain by its author. */

#include <errno.h>
#include <fenv.h>
#include <math.h>
#include <stdint.h>
#include <string.h>

#if defined(__clang__) || defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
#endif /* __clang__ || __GNUC__ */

#pragma STDC FENV_ACCESS ON

double sqrt(double x) {
    if(x <= 0.0) {
        if(x == 0.0)
            return x;

        if(math_errhandling & MATH_ERRNO)
            errno = EDOM;
        if(math_errhandling & MATH_ERREXCEPT)
            feraiseexcept(FE_INVALID);

        return nan("");
    }
    if(isnan(x) || isinf(x))
        return x;

    uint64_t ix;
    memcpy(&ix, &x, sizeof(ix));

    /* normalize 2^-511 for tee-breaker */
    double scale = 1.0;
    if(ix < 0x2000000000000000ULL) {
        x *= 0x1.0p512; /* scale up by 2^512 */
        memcpy(&ix, &x, sizeof(ix));
        scale = 0x1.0p-256; /* scale down by 2^-256 */
    }

    /* 
     * x = 2^52 * (E + Bias + m).
     * y = sqrt(x)
     * y = 2^52 * (E/2 + Bias + m/2)
     * x >> 1 = x / 2 = 2^52 * (E/2 + Bias/2 + m/2)
     * then we use the magic number y-x = 2^52 * Bias/2
     * for float64, Bias = 1023.
     * magic = 2^52 * (1023 / 2) = 2^51 * 1023 = 0x1FF8000000000000.
     */
    uint64_t iy = (ix >> 1) + 0x1FF8000000000000ULL;
    double yd;
    memcpy(&yd, &iy, sizeof(yd));

    /* refine using newton method */
    yd = 0.5 * (yd + x / yd);
    yd = 0.5 * (yd + x / yd);
    yd = 0.5 * (yd + x / yd);
    yd = 0.5 * (yd + x / yd);

    /* final refinement with fma() */
    double r = fma(-yd, yd, x);
    double h = 0.5 / yd;
    yd = fma(r, h, yd);

    /* tee breaker */
    double r_final = fma(-yd, yd, x);
    
    if (r_final < 0.0) {
        uint64_t iyd;
        memcpy(&iyd, &yd, 8);
        iyd -= 1; 
        double yd_down;
        memcpy(&yd_down, &iyd, 8);
        
        double r_down = fma(-yd_down, yd_down, x);
        if (fabs(r_down) < fabs(r_final)) {
            yd = yd_down;
        }
    } 
    else if (r_final > 0.0) {
        uint64_t iyd;
        memcpy(&iyd, &yd, 8);
        iyd += 1; 
        double yd_up;
        memcpy(&yd_up, &iyd, 8);
        
        double r_up = fma(-yd_up, yd_up, x);
        if (fabs(r_up) < fabs(r_final)) {
            yd = yd_up;
        }
    }

    return yd * scale;
}
