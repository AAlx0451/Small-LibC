/*
 * SAFE syscall() implementation
 */

#include <stdarg.h>
#include <errno.h>

#ifdef __arm__
# ifdef __APPLE__

#define REG(name, val) register long name __asm__(#name) = (long)(val)

#define _SYSCALL_GET_NTH_ARG(_1, _2, _3, _4, _5, _6, _7, _8, _9, N, ...) N
#define _SYSCALL_COUNT_ARGS(...) _SYSCALL_GET_NTH_ARG(0, ##__VA_ARGS__, 8, 7, 6, 5, 4, 3, 2, 1, 0)
#define _SYSCALL_CONCAT_IMPL(name, count) name##count
#define _SYSCALL_CONCAT(name, count) _SYSCALL_CONCAT_IMPL(name, count)

#define syscall(number, ...) \
    _SYSCALL_CONCAT(syscall, _SYSCALL_COUNT_ARGS(__VA_ARGS__))(number, ##__VA_ARGS__)

// we don't add r4-r6,r8 to clobbers, because we bind
// args to register and compiler follows ABI
#define DEFINE_SYSCALL(n, args, regs, asm_code, ...) \
long syscall##n args \
{ \
    regs \
    long error_flag; \
    \
    __asm__ volatile ( \
        asm_code \
        "mov %[err], #0\n\t" \
        "it cs\n\t" /* Thumb-2 requirement for conditional execution */ \
        "movcs %[err], #1\n\t" \
        : "+r" (r0), [err] "=r" (error_flag) \
        : __VA_ARGS__ \
        : "memory", "cc" \
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
    REG(r0, arg1); REG(r1, arg2); REG(r2, arg3); REG(r3, arg4); REG(r12, number);
    REG(r4, arg5); REG(r5, 0);,
    "push {r4, r5}\n\t" // r5 is padding
    "svc #0x80\n\t"
    "pop {r4, r5}\n\t",
    "r" (r1), "r" (r2), "r" (r3), "r" (r12), "r" (r4), "r" (r5)
)

DEFINE_SYSCALL(6, (long number, long arg1, long arg2, long arg3, long arg4, long arg5, long arg6),
    REG(r0, arg1); REG(r1, arg2); REG(r2, arg3); REG(r3, arg4); REG(r12, number);
    REG(r4, arg5); REG(r5, arg6);,
    "push {r4, r5}\n\t"
    "svc #0x80\n\t"
    "pop {r4, r5}\n\t",
    "r" (r1), "r" (r2), "r" (r3), "r" (r12), "r" (r4), "r" (r5)
)

DEFINE_SYSCALL(7, (long number, long arg1, long arg2, long arg3, long arg4, long arg5, long arg6, long arg7),
    REG(r0, arg1); REG(r1, arg2); REG(r2, arg3); REG(r3, arg4); REG(r12, number);
    REG(r4, arg5); REG(r5, arg6); REG(r6, arg7); REG(r8, 0);,
    "push {r4, r5, r6, r8}\n\t" // r8 is padding
    "svc #0x80\n\t"
    "pop {r4, r5, r6, r8}\n\t",
    "r" (r1), "r" (r2), "r" (r3), "r" (r12), "r" (r4), "r" (r5), "r" (r6), "r" (r8)
)

DEFINE_SYSCALL(8, (long number, long arg1, long arg2, long arg3, long arg4, long arg5, long arg6, long arg7, long arg8),
    REG(r0, arg1); REG(r1, arg2); REG(r2, arg3); REG(r3, arg4); REG(r12, number);
    REG(r4, arg5); REG(r5, arg6); REG(r6, arg7); REG(r8, arg8);,
    "push {r4, r5, r6, r8}\n\t"
    "svc #0x80\n\t"
    "pop {r4, r5, r6, r8}\n\t",
    "r" (r1), "r" (r2), "r" (r3), "r" (r12), "r" (r4), "r" (r5), "r" (r6), "r" (r8)
)

# endif
#endif
