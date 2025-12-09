/*
 * SAFE syscall() implementation
 * uses push and pop, which
 * are safer for clang
 * (add/str causes stack corruption)
 * be sure, that's the best way
 * to implement syscall(). I tried.
 */

/* btw it's not that I can't manage stack
 * my sub sp, sp, #8
 * str %[a5], [sp]
 * str %[a6], [sp, #4]
 * add sp, sp, #8 was correct. I think 
 */

#include <stdarg.h>
#include <errno.h>

#if defined(__GNUC__) || defined(__clang__)
# define FORCE_ARM 
#else
# define FORCE_ARM
#endif

#ifdef __arm__
# ifdef __APPLE__

#define REG(name, val) register long name __asm__(#name) = val

#define _SYSCALL_GET_NTH_ARG(_1, _2, _3, _4, _5, _6, _7, _8, _9, N, ...) N
#define _SYSCALL_COUNT_ARGS(...) _SYSCALL_GET_NTH_ARG(0, ##__VA_ARGS__, 8, 7, 6, 5, 4, 3, 2, 1, 0)
#define _SYSCALL_CONCAT_IMPL(name, count) name##count
#define _SYSCALL_CONCAT(name, count) _SYSCALL_CONCAT_IMPL(name, count)

#define syscall(number, ...) \
    _SYSCALL_CONCAT(syscall, _SYSCALL_COUNT_ARGS(__VA_ARGS__))(number, ##__VA_ARGS__)

#define DEFINE_SYSCALL(n, args, regs, asm_code, ...) \
FORCE_ARM \
long syscall##n args \
{ \
    regs \
    long error_flag; \
    \
    __asm__ volatile ( \
        asm_code \
        "mov %[err], #0\n\t" \
        "it cs\n\t"          /* Thumb-2 requirement for conditional execution */ \
        "movcs %[err], #1\n\t" \
        : "+r" (r0), [err] "=r" (error_flag) \
        : __VA_ARGS__ \
        : "memory", "cc", "r4", "r5", "r6", "r8" \
    ); \
    \
    if (error_flag) { \
        errno = (int)r0; \
        return -1; \
    } \
    return r0; \
}

DEFINE_SYSCALL(0, (long number),
    REG(r0, 0); REG(r12, number);,
    "svc #0x80\n\t",
    "r" (r12)
)

DEFINE_SYSCALL(1, (long number, long arg1),
    REG(r0, arg1); REG(r12, number);,
    "svc #0x80\n\t",
    "r" (r12)
)

DEFINE_SYSCALL(2, (long number, long arg1, long arg2),
    REG(r0, arg1); REG(r1, arg2); REG(r12, number);,
    "svc #0x80\n\t",
    "r" (r1), "r" (r12)
)

DEFINE_SYSCALL(3, (long number, long arg1, long arg2, long arg3),
    REG(r0, arg1); REG(r1, arg2); REG(r2, arg3); REG(r12, number);,
    "svc #0x80\n\t",
    "r" (r1), "r" (r2), "r" (r12)
)

DEFINE_SYSCALL(4, (long number, long arg1, long arg2, long arg3, long arg4),
    REG(r0, arg1); REG(r1, arg2); REG(r2, arg3); REG(r3, arg4); REG(r12, number);,
    "svc #0x80\n\t",
    "r" (r1), "r" (r2), "r" (r3), "r" (r12)
)

DEFINE_SYSCALL(5, (long number, long arg1, long arg2, long arg3, long arg4, long arg5),
    REG(r0, arg1); REG(r1, arg2); REG(r2, arg3); REG(r3, arg4); REG(r12, number);,
    "push {r4}\n\t"
    "mov r4, %[a5]\n\t"
    "push {r4}\n\t"
    "svc #0x80\n\t"
    "pop {r4}\n\t"
    "pop {r4}\n\t",
    "r" (r1), "r" (r2), "r" (r3), "r" (r12), [a5] "r" (arg5)
)

DEFINE_SYSCALL(6, (long number, long arg1, long arg2, long arg3, long arg4, long arg5, long arg6),
    REG(r0, arg1); REG(r1, arg2); REG(r2, arg3); REG(r3, arg4); REG(r12, number);,
    "push {r5}\n\t"
    "push {r4}\n\t"
    "mov r5, %[a6]\n\t"
    "mov r4, %[a5]\n\t"
    "push {r5}\n\t"
    "push {r4}\n\t"
    "svc #0x80\n\t"
    "pop {r4}\n\t"
    "pop {r5}\n\t"
    "pop {r4}\n\t"
    "pop {r5}\n\t",
    "r" (r1), "r" (r2), "r" (r3), "r" (r12), [a5] "r" (arg5), [a6] "r" (arg6)
)

DEFINE_SYSCALL(7, (long number, long arg1, long arg2, long arg3, long arg4, long arg5, long arg6, long arg7),
    REG(r0, arg1); REG(r1, arg2); REG(r2, arg3); REG(r3, arg4); REG(r12, number);,
    "push {r6}\n\t"
    "push {r5}\n\t"
    "push {r4}\n\t"
    "mov r6, %[a7]\n\t"
    "mov r5, %[a6]\n\t"
    "mov r4, %[a5]\n\t"
    "push {r6}\n\t"
    "push {r5}\n\t"
    "push {r4}\n\t"
    "svc #0x80\n\t"
    "pop {r4}\n\t"
    "pop {r5}\n\t"
    "pop {r6}\n\t"
    "pop {r4}\n\t"
    "pop {r5}\n\t"
    "pop {r6}\n\t",
    "r" (r1), "r" (r2), "r" (r3), "r" (r12), [a5] "r" (arg5), [a6] "r" (arg6), [a7] "r" (arg7)
)

DEFINE_SYSCALL(8, (long number, long arg1, long arg2, long arg3, long arg4, long arg5, long arg6, long arg7, long arg8),
    REG(r0, arg1); REG(r1, arg2); REG(r2, arg3); REG(r3, arg4); REG(r12, number);,
    "push {r8}\n\t"
    "push {r6}\n\t"
    "push {r5}\n\t"
    "push {r4}\n\t"
    "mov r8, %[a8]\n\t"
    "mov r6, %[a7]\n\t"
    "mov r5, %[a6]\n\t"
    "mov r4, %[a5]\n\t"
    "push {r8}\n\t"
    "push {r6}\n\t"
    "push {r5}\n\t"
    "push {r4}\n\t"
    "svc #0x80\n\t"
    "pop {r4}\n\t"
    "pop {r5}\n\t"
    "pop {r6}\n\t"
    "pop {r8}\n\t"
    "pop {r4}\n\t"
    "pop {r5}\n\t"
    "pop {r6}\n\t"
    "pop {r8}\n\t",
    "r" (r1), "r" (r2), "r" (r3), "r" (r12), [a5] "r" (arg5), [a6] "r" (arg6), [a7] "r" (arg7), [a8] "r" (arg8)
)

# endif
#endif
