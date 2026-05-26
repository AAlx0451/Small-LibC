#ifndef SRC_FENV_IMPL_H
#define SRC_FENV_IMPL_H

#if !defined(__clang__) && !defined(__GNUC__)
# error fenv.h not supported for non-gnu-style compilers. see src/fenv/impl.h
#endif /* !__clang__ && !__GNUC__ */

#include <stdint.h>

static inline uint32_t _get_fp_status(void)
{
    uint32_t sw = 0;
#if defined(__x86_64__) || defined(__i386__)
    __asm__ __volatile__("stmxcsr %0" : "=m"(sw));
#elif defined(__aarch64__)
    uint64_t wsw = sw;
    __asm__ __volatile__("mrs %0, fpsr" : "=r"(wsw));
    sw = wsw;
#elif defined(__arm__)
    __asm__ __volatile__("vmrs %0, fpscr" : "=r"(sw));
#elif defined(__riscv)
    __asm__ __volatile__("frcsr %0" : "=r"(sw));
#elif defined(__powerpc__) || defined(__ppc__)
    double fpscr;
    __asm__ __volatile__("mffs %0" : "=f"(fpscr));
    sw = (uint32_t)*(uint64_t *)&fpscr;
#elif defined(__sparc__)
    __asm__ __volatile__("st %%fsr, %0" : "=m"(sw));
#endif
    return sw;
}

static inline void _set_fp_status(uint32_t sw)
{
#if defined(__x86_64__) || defined(__i386__)
    __asm__ __volatile__("ldmxcsr %0" : : "m"(sw));
#elif defined(__aarch64__)
    uint64_t wsw = sw;
    __asm__ __volatile__("msr fpsr, %0" : : "r"(wsw));
    sw = wsw;
#elif defined(__arm__)
    __asm__ __volatile__("vmsr fpscr, %0" : : "r"(sw));
#elif defined(__riscv)
    __asm__ __volatile__("fscsr %0" : : "r"(sw));
#elif defined(__powerpc__) || defined(__ppc__)
    double fpscr;
    *(uint64_t *)&fpscr = sw;
    __asm__ __volatile__("mtfsf 255, %0" : : "f"(fpscr));
#elif defined(__sparc__)
    __asm__ __volatile__("ld %0, %%fsr" : : "m"(sw));
#endif
}

/* aarch64 has separated control and status */
static inline uint32_t _get_fp_control(void)
{
#if defined(__aarch64__)
    uint32_t cw;
    uint64_t wcw = cw;
    __asm__ __volatile__("mrs %0, fpcr" : "=r"(wcw));
    cw = wcw;
    return cw;
#else
    return _get_fp_status();
#endif
}

static inline void _set_fp_control(uint32_t cw)
{
#if defined(__aarch64__)
    uint64_t wcw = cw;
    __asm__ __volatile__("msr fpcr, %0" : : "r"(wcw));
#else
    _set_fp_status(cw);
#endif
}

#endif /* !SRC_FENV_IMPL_H */
