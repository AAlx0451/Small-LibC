#include "impl.h"
#include <fenv.h>

int fegetround(void) {
    uint32_t cw = _get_fp_control();
    return cw & (FE_TONEAREST | FE_DOWNWARD | FE_UPWARD | FE_TOWARDZERO);
}
