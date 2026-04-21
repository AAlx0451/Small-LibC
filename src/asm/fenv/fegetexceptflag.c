#include "impl.h"
#include <fenv.h>

int fegetexceptflag(fexcept_t *flagp, int excepts) {
    uint32_t sw = _get_fp_status();
    *flagp = (fexcept_t)(sw & (unsigned)excepts & FE_ALL_EXCEPT);
    return 0;
}
