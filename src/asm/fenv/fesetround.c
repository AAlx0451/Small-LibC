#include "impl.h"
#include <fenv.h>

int fesetround(int round)
{
    if ((round & ~(FE_TONEAREST | FE_DOWNWARD | FE_UPWARD | FE_TOWARDZERO)) != 0) {
        return 1; /* invalid argument */
    }

    uint32_t cw = _get_fp_control();
    cw &= ~(uint32_t)(FE_TONEAREST | FE_DOWNWARD | FE_UPWARD | FE_TOWARDZERO);
    cw |= (uint32_t)round;
    _set_fp_control(cw);
    return 0;
}
