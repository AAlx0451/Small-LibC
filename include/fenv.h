/* This file has been put into the public domain by its author. */

#ifndef _FENV_H
#define _FENV_H

#if defined(__x86_64__) || defined(__i386__)
    /* x86 SSE MXCSR Register */
    #define FE_INVALID    0x01
    #define FE_DIVBYZERO  0x04
    #define FE_OVERFLOW   0x08
    #define FE_UNDERFLOW  0x10
    #define FE_INEXACT    0x20

    #define FE_TONEAREST  0x0000
    #define FE_DOWNWARD   0x0400
    #define FE_UPWARD     0x0800
    #define FE_TOWARDZERO 0x0c00

#elif defined(__aarch64__) || defined(__arm__)
    /* ARMv7 FPSCR / AArch64 FPCR/FPSR */
    #define FE_INVALID    0x01
    #define FE_DIVBYZERO  0x02
    #define FE_OVERFLOW   0x04
    #define FE_UNDERFLOW  0x08
    #define FE_INEXACT    0x10

    #define FE_TONEAREST  0x00000000
    #define FE_UPWARD     0x00400000
    #define FE_DOWNWARD   0x00800000
    #define FE_TOWARDZERO 0x00c00000

#elif defined(__riscv)
    /* RISC-V FCSR Register */
    #define FE_INEXACT    0x01
    #define FE_UNDERFLOW  0x02
    #define FE_OVERFLOW   0x04
    #define FE_DIVBYZERO  0x08
    #define FE_INVALID    0x10

    #define FE_TONEAREST  0x000
    #define FE_TOWARDZERO 0x020
    #define FE_DOWNWARD   0x040
    #define FE_UPWARD     0x060

#elif defined(__powerpc__) || defined(__ppc__)
    /* PowerPC FPSCR Register */
    #define FE_INEXACT    0x02000000
    #define FE_DIVBYZERO  0x04000000
    #define FE_UNDERFLOW  0x08000000
    #define FE_OVERFLOW   0x10000000
    #define FE_INVALID    0x20000000

    #define FE_TONEAREST  0x00000000
    #define FE_TOWARDZERO 0x00000001
    #define FE_UPWARD     0x00000002
    #define FE_DOWNWARD   0x00000003

#elif defined(__sparc__)
    /* SPARC FSR Register */
    #define FE_INEXACT    0x01
    #define FE_DIVBYZERO  0x02
    #define FE_UNDERFLOW  0x04
    #define FE_OVERFLOW   0x08
    #define FE_INVALID    0x10

    #define FE_TONEAREST  0x00000000
    #define FE_TOWARDZERO 0x40000000
    #define FE_UPWARD     0x80000000
    #define FE_DOWNWARD   0xc0000000

#else
    #error "Architecture not supported for fenv.h"
#endif

#define FE_ALL_EXCEPT (FE_INEXACT | FE_DIVBYZERO | FE_UNDERFLOW | FE_OVERFLOW | FE_INVALID)

typedef unsigned int fexcept_t;

typedef struct {
    unsigned int __cw; /* Control word */
    unsigned int __sw; /* Status word */
} fenv_t;

extern const fenv_t _FE_DFL_ENV;

#define FE_DFL_ENV ((const fenv_t *)&_FE_DFL_ENV)

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

int feclearexcept(int excepts);
int fegetexceptflag(fexcept_t *flagp, int excepts);
int feraiseexcept(int excepts);
int fesetexceptflag(const fexcept_t *flagp, int excepts);
int fetestexcept(int excepts);
int fegetround(void);
int fesetround(int round);
int fegetenv(fenv_t *envp);
int feholdexcept(fenv_t *envp);
int fesetenv(const fenv_t *envp);
int feupdateenv(const fenv_t *envp);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !_FENV_H */
