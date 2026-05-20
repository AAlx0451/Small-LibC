#include "impl.h"
#include <fenv.h>

int feclearexcept(int excepts)
{
    uint32_t sw = _get_fp_status();
    sw &= ~(excepts & FE_ALL_EXCEPT);
    _set_fp_status(sw);
    return 0;
}
