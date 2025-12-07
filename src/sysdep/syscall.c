/*
 * SAFE syscall() implementation
 * uses push and pop, which
 * are safer for clang
 * (add/str causes stack corruption
 */

/* this implementation is SAFE!!! */

#include <stdarg.h>
#include <errno.h>

#if defined(__GNUC__) || defined(__clang__)
# define FORCE_ARM __attribute__((target("arm")))
#else
# define FORCE_ARM
#endif

#ifdef __arm__
# ifdef __APPLE__

#define ASM_SYSCALL_TAIL(stack_bytes, restore_regs) \
    "svc #0x80\n\t" \
    "add sp, sp, %0\n\t" \
    "mov %[err], #0\n\t" \
    "movcs %[err], #1\n\t" \
    restore_regs \
    : "+r" (r0), [err] "=r" (error_flag)

#define RETURN_RESULT \
    if (error_flag) { \
        errno = (int)r0; \
        return -1; \
    } \
    return r0;

FORCE_ARM
long syscall(long number, ...)
{
    va_list args;
    long arg1, arg2, arg3, arg4, arg5, arg6;
    long error_flag;

    va_start(args, number);
    arg1 = va_arg(args, long);
    arg2 = va_arg(args, long);
    arg3 = va_arg(args, long);
    arg4 = va_arg(args, long);
    arg5 = va_arg(args, long);
    arg6 = va_arg(args, long);
    va_end(args);

    register long r0 __asm__("r0") = arg1;
    register long r1 __asm__("r1") = arg2;
    register long r2 __asm__("r2") = arg3;
    register long r3 __asm__("r3") = arg4;
    register long r12 __asm__("r12") = number;

    __asm__ volatile (
        "push {%[a6]}\n\t"
        "push {%[a5]}\n\t"
        "svc #0x80\n\t"
        "pop {r1, r2}\n\t"
        "mov %[err], #0\n\t"
        "movcs %[err], #1\n\t"
        : "+r" (r0), [err] "=r" (error_flag)
        : "r" (r1), "r" (r2), "r" (r3), "r" (r12),
          [a5] "r" (arg5), [a6] "r" (arg6)
        : "memory", "cc"
    );
    RETURN_RESULT
}

FORCE_ARM
long syscall0(long number)
{
    long error_flag;
    register long r0 __asm__("r0") = 0;
    register long r12 __asm__("r12") = number;

    __asm__ volatile (
        "svc #0x80\n\t"
        "mov %[err], #0\n\t"
        "movcs %[err], #1\n\t"
        : "+r" (r0), [err] "=r" (error_flag)
        : "r" (r12)
        : "memory", "cc"
    );
    RETURN_RESULT
}

FORCE_ARM
long syscall1(long number, long arg1)
{
    long error_flag;
    register long r0 __asm__("r0") = arg1;
    register long r12 __asm__("r12") = number;

    __asm__ volatile (
        "svc #0x80\n\t"
        "mov %[err], #0\n\t"
        "movcs %[err], #1\n\t"
        : "+r" (r0), [err] "=r" (error_flag)
        : "r" (r12)
        : "memory", "cc"
    );
    RETURN_RESULT
}

FORCE_ARM
long syscall2(long number, long arg1, long arg2)
{
    long error_flag;
    register long r0 __asm__("r0") = arg1;
    register long r1 __asm__("r1") = arg2;
    register long r12 __asm__("r12") = number;

    __asm__ volatile (
        "svc #0x80\n\t"
        "mov %[err], #0\n\t"
        "movcs %[err], #1\n\t"
        : "+r" (r0), [err] "=r" (error_flag)
        : "r" (r1), "r" (r12)
        : "memory", "cc"
    );
    RETURN_RESULT
}

FORCE_ARM
long syscall3(long number, long arg1, long arg2, long arg3)
{
    long error_flag;
    register long r0 __asm__("r0") = arg1;
    register long r1 __asm__("r1") = arg2;
    register long r2 __asm__("r2") = arg3;
    register long r12 __asm__("r12") = number;

    __asm__ volatile (
        "svc #0x80\n\t"
        "mov %[err], #0\n\t"
        "movcs %[err], #1\n\t"
        : "+r" (r0), [err] "=r" (error_flag)
        : "r" (r1), "r" (r2), "r" (r12)
        : "memory", "cc"
    );
    RETURN_RESULT
}

FORCE_ARM
long syscall4(long number, long arg1, long arg2, long arg3, long arg4)
{
    long error_flag;
    register long r0 __asm__("r0") = arg1;
    register long r1 __asm__("r1") = arg2;
    register long r2 __asm__("r2") = arg3;
    register long r3 __asm__("r3") = arg4;
    register long r12 __asm__("r12") = number;

    __asm__ volatile (
        "svc #0x80\n\t"
        "mov %[err], #0\n\t"
        "movcs %[err], #1\n\t"
        : "+r" (r0), [err] "=r" (error_flag)
        : "r" (r1), "r" (r2), "r" (r3), "r" (r12)
        : "memory", "cc"
    );
    RETURN_RESULT
}

FORCE_ARM
long syscall5(long number, long arg1, long arg2, long arg3, long arg4, long arg5)
{
    long error_flag;
    register long r0 __asm__("r0") = arg1;
    register long r1 __asm__("r1") = arg2;
    register long r2 __asm__("r2") = arg3;
    register long r3 __asm__("r3") = arg4;
    register long r12 __asm__("r12") = number;

    __asm__ volatile (
        "push {%[a5]}\n\t"
        "svc #0x80\n\t"
        "pop {r1}\n\t"
        "mov %[err], #0\n\t"
        "movcs %[err], #1\n\t"
        : "+r" (r0), [err] "=r" (error_flag)
        : "r" (r1), "r" (r2), "r" (r3), "r" (r12), [a5] "r" (arg5)
        : "memory", "cc"
    );
    RETURN_RESULT
}

FORCE_ARM
long syscall6(long number, long arg1, long arg2, long arg3, long arg4, long arg5, long arg6)
{
    long error_flag;
    register long r0 __asm__("r0") = arg1;
    register long r1 __asm__("r1") = arg2;
    register long r2 __asm__("r2") = arg3;
    register long r3 __asm__("r3") = arg4;
    register long r12 __asm__("r12") = number;

    __asm__ volatile (
        "push {%[a6]}\n\t"
        "push {%[a5]}\n\t"
        "svc #0x80\n\t"
        "pop {r1, r2}\n\t"
        "mov %[err], #0\n\t"
        "movcs %[err], #1\n\t"
        : "+r" (r0), [err] "=r" (error_flag)
        : "r" (r1), "r" (r2), "r" (r3), "r" (r12),
          [a5] "r" (arg5), [a6] "r" (arg6)
        : "memory", "cc"
    );
    RETURN_RESULT
}

FORCE_ARM
long syscall8(long number, long arg1, long arg2, long arg3, long arg4,
              long arg5, long arg6, long arg7, long arg8)
{
    long error_flag;
    register long r0 __asm__("r0") = arg1;
    register long r1 __asm__("r1") = arg2;
    register long r2 __asm__("r2") = arg3;
    register long r3 __asm__("r3") = arg4;
    register long r12 __asm__("r12") = number;

    __asm__ volatile (
        "push {%[a8]}\n\t"
        "push {%[a7]}\n\t"
        "push {%[a6]}\n\t"
        "push {%[a5]}\n\t"
        "svc #0x80\n\t"
        "pop {r1, r2, r3, r12}\n\t"
        "mov %[err], #0\n\t"
        "movcs %[err], #1\n\t"
        : "+r" (r0), [err] "=r" (error_flag)
        : "r" (r1), "r" (r2), "r" (r3), "r" (r12),
          [a5] "r" (arg5), [a6] "r" (arg6), [a7] "r" (arg7), [a8] "r" (arg8)
        : "memory", "cc"
    );
    RETURN_RESULT
}

# endif
#endif
