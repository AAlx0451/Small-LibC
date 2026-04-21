#include "impl.h"
#include <fenv.h>

int fegetenv(fenv_t *envp) {
    envp->__sw = _get_fp_status();
    envp->__cw = _get_fp_control();
    return 0;
}
