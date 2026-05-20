#include <fenv.h>
#include <float.h>

/* simple helper to avoid compiler optimizations */
static void _force_eval_double(double x)
{
    volatile double y = x;
    (void)y;
}

int feraiseexcept(int excepts)
{
    excepts &= FE_ALL_EXCEPT;
    if (excepts == 0) {
        return 0;
    }

    /*
     * 1. Invalid
     * 2. Divide by Zero
     * 3. Overflow / Underflow
     * 4. Inexact
     */

    if (excepts & FE_INVALID) {
        volatile double zero = 0.0;
        _force_eval_double(zero / zero);
    }

    if (excepts & FE_DIVBYZERO) {
        volatile double one = 1.0;
        volatile double zero = 0.0;
        _force_eval_double(one / zero);
    }

    if (excepts & FE_OVERFLOW) {
        volatile double max = DBL_MAX;
        _force_eval_double(max * max);
    }

    if (excepts & FE_UNDERFLOW) {
        volatile double min = DBL_MIN;
        _force_eval_double(min * min);
    }

    if (excepts & FE_INEXACT) {
        volatile double two = 2.0;
        volatile double three = 3.0;
        _force_eval_double(two / three);
    }

    return 0;
}
