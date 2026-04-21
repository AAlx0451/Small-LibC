#include "impl.h"
#include <fenv.h>

int fesetenv(const fenv_t *envp) {
    if(envp == FE_DFL_ENV) {
        uint32_t cw = _get_fp_control();
        cw &= ~(uint32_t)(FE_ALL_EXCEPT | FE_TONEAREST | FE_DOWNWARD | FE_UPWARD | FE_TOWARDZERO);
        cw |= (uint32_t)FE_TONEAREST;
        _set_fp_control(cw);

        uint32_t sw = _get_fp_status();
        sw &= ~(uint32_t)FE_ALL_EXCEPT;
        _set_fp_status(sw);
    } else {
        _set_fp_control(envp->__cw);
        _set_fp_status(envp->__sw);
    }
    return 0;
}
