#include "impl.h"
#include <fenv.h>

int feholdexcept(fenv_t *envp) {
    fegetenv(envp);
    feclearexcept(FE_ALL_EXCEPT);
    uint32_t cw = _get_fp_control();

#if defined(__x86_64__) || defined(__i386__)

    cw |= (uint32_t)(FE_ALL_EXCEPT << 7);

#elif defined(__aarch64__) || defined(__arm__)

    cw &= ~(uint32_t)(FE_ALL_EXCEPT << 8);

#elif defined(__riscv)

    /* always nonstop */

#elif defined(__powerpc__) || defined(__ppc__)

    cw &= ~0x000000F8U;

#elif defined(__sparc__)

    cw &= ~(0x1FU << 23);

#endif /* __x86_64__ || __i386__ */

    _set_fp_control(cw);
    return 0;
}
