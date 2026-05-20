#include "impl.h"
#include <fenv.h>

int fetestexcept(int excepts)
{
    uint32_t sw = _get_fp_status();
    return (sw & (unsigned)excepts & FE_ALL_EXCEPT);
}
