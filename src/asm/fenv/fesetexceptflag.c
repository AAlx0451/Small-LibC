#include "impl.h"
#include <fenv.h>

int fesetexceptflag(const fexcept_t *flagp, int excepts)
{
    uint32_t sw = _get_fp_status();
    sw &= ~(excepts & FE_ALL_EXCEPT);
    sw |= (*flagp & (unsigned)excepts & FE_ALL_EXCEPT);
    _set_fp_status(sw);
    return 0;
}
