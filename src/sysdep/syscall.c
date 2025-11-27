#include <stdarg.h>
#include <errno.h>

#ifdef __arm__
# ifdef __APPLE__

long syscall(long number, ...) /* we use long, which is redundant */
{
    va_list args;
    long arg1, arg2, arg3, arg4, arg5, arg6;
    long error_flag;

    /* extract args and pin them to registers */
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
    register long r12 __asm__("r12") = number; /* syscall number register */

    __asm__ volatile (
        "sub sp, sp, #8\n\t"        /* Allocate stack for arg5, arg6 */
        "str %[a5], [sp]\n\t"       /* Store arg5 */
        "str %[a6], [sp, #4]\n\t"   /* Store arg6 */
        "svc #0x80\n\t"             /* Execute syscall */
        "add sp, sp, #8\n\t"        /* Restore stack */
        "mov %[err], #0\n\t"        /* Reset error flag */
        "movcs %[err], #1\n\t"      /* Set error flag if Carry Bit is set */
        : "+r" (r0), [err] "=r" (error_flag)
        : "r" (r1), "r" (r2), "r" (r3), "r" (r12), [a5] "r" (arg5), [a6] "r" (arg6)
        : "memory", "cc"
    );

    // Error handling
    if (error_flag)
    {
        errno = (int)r0;
        return -1;
    }

    return r0;
}

long syscall0(long number)
{
    long error_flag;
    register long r0 __asm__("r0") = 0; /* Dummy init for output */
    register long r12 __asm__("r12") = number;

    __asm__ volatile (
        "svc #0x80\n\t"             /* Execute syscall */
        "mov %[err], #0\n\t"        /* Reset error flag */
        "movcs %[err], #1\n\t"      /* Set error flag if Carry Bit is set */
        : "+r" (r0), [err] "=r" (error_flag)
        : "r" (r12)
        : "memory", "cc"
    );

    if (error_flag)
    {
        errno = (int)r0;
        return -1;
    }

    return r0;
}

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

    if (error_flag)
    {
        errno = (int)r0;
        return -1;
    }

    return r0;
}

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

    if (error_flag)
    {
        errno = (int)r0;
        return -1;
    }

    return r0;
}

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

    if (error_flag)
    {
        errno = (int)r0;
        return -1;
    }

    return r0;
}

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

    if (error_flag)
    {
        errno = (int)r0;
        return -1;
    }

    return r0;
}

long syscall5(long number, long arg1, long arg2, long arg3, long arg4, long arg5)
{
    long error_flag;
    register long r0 __asm__("r0") = arg1;
    register long r1 __asm__("r1") = arg2;
    register long r2 __asm__("r2") = arg3;
    register long r3 __asm__("r3") = arg4;
    register long r12 __asm__("r12") = number;

    __asm__ volatile (
        "sub sp, sp, #4\n\t"        /* Allocate stack for arg5 */
        "str %[a5], [sp]\n\t"       /* Store arg5 */
        "svc #0x80\n\t"
        "add sp, sp, #4\n\t"        /* Restore stack */
        "mov %[err], #0\n\t"
        "movcs %[err], #1\n\t"
        : "+r" (r0), [err] "=r" (error_flag)
        : "r" (r1), "r" (r2), "r" (r3), "r" (r12), [a5] "r" (arg5)
        : "memory", "cc"
    );

    if (error_flag)
    {
        errno = (int)r0;
        return -1;
    }

    return r0;
}

long syscall6(long number, long arg1, long arg2, long arg3, long arg4, long arg5, long arg6)
{
    long error_flag;
    register long r0 __asm__("r0") = arg1;
    register long r1 __asm__("r1") = arg2;
    register long r2 __asm__("r2") = arg3;
    register long r3 __asm__("r3") = arg4;
    register long r12 __asm__("r12") = number;

    __asm__ volatile (
        "sub sp, sp, #8\n\t"        /* Allocate stack for arg5, arg6 */
        "str %[a5], [sp]\n\t"       /* Store arg5 */
        "str %[a6], [sp, #4]\n\t"   /* Store arg6 */
        "svc #0x80\n\t"
        "add sp, sp, #8\n\t"        /* Restore stack */
        "mov %[err], #0\n\t"
        "movcs %[err], #1\n\t"
        : "+r" (r0), [err] "=r" (error_flag)
        : "r" (r1), "r" (r2), "r" (r3), "r" (r12), [a5] "r" (arg5), [a6] "r" (arg6)
        : "memory", "cc"
    );

    if (error_flag)
    {
        errno = (int)r0;
        return -1;
    }

    return r0;
}

# endif
# ifdef __linux__
#  error Linux not supported now
# endif

#else
# error Architecture not supported
#endif
