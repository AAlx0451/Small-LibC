#ifndef USELESS_SYSCALL_H
#define USELESS_SYSCALL_H

/*
 * SAFE syscall() implementation
 * target: iOS 6, ARMv7 (thumb-2 preferred, UAL ARM as well), clang(!!!!!!! not for gcc !!!!!!!!)
 */

#include <errno.h>

#ifdef __arm__
# ifdef __APPLE__

/*
 * simple helper
 * aligns stack by 16 bytes (pushing 4 regs) and marks stack changes for CFI
 */
__asm__ ( // static __attribute__("naked") void do_syscall(void)
    ".text\n\t"
    ".align 4\n\t"
    ".thumb_func _do_syscall\n\t"
    "_do_syscall:\n\t"
    ".cfi_startproc\n\t"
    "push {r4, r5, r6, r8}\n\t" // r7 is a frame pointer (actually, it will protect CFI if code will break :D)
    ".cfi_def_cfa_offset 16\n\t"
    ".cfi_offset r4, -16\n\t"
    ".cfi_offset r5, -12\n\t"
    ".cfi_offset r6, -8\n\t"
    ".cfi_offset r8, -4\n\t"
    "svc #128\n\t"
    "pop {r4, r5, r6, r8}\n\t"
    ".cfi_def_cfa_offset 0\n\t"
    "bx lr\n\t"
    ".cfi_endproc\n"
);

/*
 * Syscall function overloads.
 */

/*
 * Performs a syscall with 0 arguments.
 */

static inline __attribute__((overloadable, always_inline))
long syscall(long number)
{
    register long r0 __asm__("r0") = 0;
    register long r12 __asm__("r12") = (long)number;
    long error_flag;

    __asm__ volatile (
        "svc #0x80\n\t"
        "mov %[err], #0\n\t"
        "it cs\n\t"
        "movcs %[err], #1\n\t"
        : "+r" (r0), [err] "=r" (error_flag)
        : "r" (r12)
        : "memory", "cc", "lr"
    );

    if (error_flag) {
        errno = (int)r0;
        return -1;
    }
    return r0;
}

/*
 * Performs a syscall with 1 argument.
 */

static inline __attribute__((overloadable, always_inline))
long syscall(long number, long arg1)
{
    register long r0 __asm__("r0") = (long)arg1;
    register long r12 __asm__("r12") = (long)number;
    long error_flag;

    __asm__ volatile (
        "svc #0x80\n\t"
        "mov %[err], #0\n\t"
        "it cs\n\t"
        "movcs %[err], #1\n\t"
        : "+r" (r0), [err] "=r" (error_flag)
        : "r" (r12)
        : "memory", "cc", "lr"
    );

    if (error_flag) {
        errno = (int)r0;
        return -1;
    }
    return r0;
}

/*
 * Performs a syscall with 2 arguments.
 */

static inline __attribute__((overloadable, always_inline))
long syscall(long number, long arg1, long arg2)
{
    register long r0 __asm__("r0") = (long)arg1;
    register long r1 __asm__("r1") = (long)arg2;
    register long r12 __asm__("r12") = (long)number;
    long error_flag;

    __asm__ volatile (
        "svc #0x80\n\t"
        "mov %[err], #0\n\t"
        "it cs\n\t"
        "movcs %[err], #1\n\t"
        : "+r" (r0), [err] "=r" (error_flag)
        : "r" (r1), "r" (r12)
        : "memory", "cc", "lr"
    );

    if (error_flag) {
        errno = (int)r0;
        return -1;
    }
    return r0;
}

/*
 * Performs a syscall with 3 arguments.
 */
static inline __attribute__((overloadable, always_inline))
long syscall(long number, long arg1, long arg2, long arg3)
{
    register long r0 __asm__("r0") = (long)arg1;
    register long r1 __asm__("r1") = (long)arg2;
    register long r2 __asm__("r2") = (long)arg3;
    register long r12 __asm__("r12") = (long)number;
    long error_flag;

    __asm__ volatile (
        "svc #0x80\n\t"
        "mov %[err], #0\n\t"
        "it cs\n\t"
        "movcs %[err], #1\n\t"
        : "+r" (r0), [err] "=r" (error_flag)
        : "r" (r1), "r" (r2), "r" (r12)
        : "memory", "cc", "lr"
    );

    if (error_flag) {
        errno = (int)r0;
        return -1;
    }
    return r0;
}

/*
 * Performs a syscall with 4 arguments.
 */

static inline __attribute__((overloadable, always_inline))
long syscall(long number, long arg1, long arg2, long arg3, long arg4)
{
    register long r0 __asm__("r0") = (long)arg1;
    register long r1 __asm__("r1") = (long)arg2;
    register long r2 __asm__("r2") = (long)arg3;
    register long r3 __asm__("r3") = (long)arg4;
    register long r12 __asm__("r12") = (long)number;
    long error_flag;

    __asm__ volatile (
        "svc #0x80\n\t"
        "mov %[err], #0\n\t"
        "it cs\n\t"
        "movcs %[err], #1\n\t"
        : "+r" (r0), [err] "=r" (error_flag)
        : "r" (r1), "r" (r2), "r" (r3), "r" (r12)
        : "memory", "cc", "lr"
    );

    if (error_flag) {
        errno = (int)r0;
        return -1;
    }
    return r0;
}

/*
 * Performs a syscall with 5 arguments
 */

static inline __attribute__((overloadable, always_inline))
long syscall(long number, long arg1, long arg2, long arg3, long arg4, long arg5)
{
    register long r0 __asm__("r0") = (long)arg1;
    register long r1 __asm__("r1") = (long)arg2;
    register long r2 __asm__("r2") = (long)arg3;
    register long r3 __asm__("r3") = (long)arg4;
    register long r12 __asm__("r12") = (long)number;
    register long r4 __asm__("r4") = (long)arg5;
    register long r5 __asm__("r5") = 0;
    register long r6 __asm__("r6") = 0;
    register long r8 __asm__("r8") = 0;
    long error_flag;

    __asm__ volatile (
        "bl _do_syscall\n\t"
        "mov %[err], #0\n\t"
        "it cs\n\t"
        "movcs %[err], #1\n\t"
        : "+r" (r0), [err] "=r" (error_flag)
        : "r" (r1), "r" (r2), "r" (r3), "r" (r12), "r" (r4), "r" (r5), "r" (r6), "r" (r8)
        : "memory", "cc", "lr"
    );

    if (error_flag) {
        errno = (int)r0;
        return -1;
    }
    return r0;
}

/*
 * Performs a syscall with 6 arguments
 */
static inline __attribute__((overloadable, always_inline))
long syscall(long number, long arg1, long arg2, long arg3, long arg4, long arg5, long arg6)
{
    register long r0 __asm__("r0") = (long)arg1;
    register long r1 __asm__("r1") = (long)arg2;
    register long r2 __asm__("r2") = (long)arg3;
    register long r3 __asm__("r3") = (long)arg4;
    register long r12 __asm__("r12") = (long)number;
    register long r4 __asm__("r4") = (long)arg5;
    register long r5 __asm__("r5") = (long)arg6;
    register long r6 __asm__("r6") = 0;
    register long r8 __asm__("r8") = 0;
    long error_flag;

    __asm__ volatile (
        "bl _do_syscall\n\t"
        "mov %[err], #0\n\t"
        "it cs\n\t"
        "movcs %[err], #1\n\t"
        : "+r" (r0), [err] "=r" (error_flag)
        : "r" (r1), "r" (r2), "r" (r3), "r" (r12), "r" (r4), "r" (r5), "r" (r6), "r" (r8)
        : "memory", "cc", "lr"
    );

    if (error_flag) {
        errno = (int)r0;
        return -1;
    }
    return r0;
}

/*
 * Performs a syscall with 7 arguments
 */
static inline __attribute__((overloadable, always_inline))
long syscall(long number, long arg1, long arg2, long arg3, long arg4, long arg5, long arg6, long arg7)
{
    register long r0 __asm__("r0") = (long)arg1;
    register long r1 __asm__("r1") = (long)arg2;
    register long r2 __asm__("r2") = (long)arg3;
    register long r3 __asm__("r3") = (long)arg4;
    register long r12 __asm__("r12") = (long)number;
    register long r4 __asm__("r4") = (long)arg5;
    register long r5 __asm__("r5") = (long)arg6;
    register long r6 __asm__("r6") = (long)arg7;
    register long r8 __asm__("r8") = 0;
    long error_flag;

    __asm__ volatile (
        "bl _do_syscall\n\t"
        "mov %[err], #0\n\t"
        "it cs\n\t"
        "movcs %[err], #1\n\t"
        : "+r" (r0), [err] "=r" (error_flag)
        : "r" (r1), "r" (r2), "r" (r3), "r" (r12), "r" (r4), "r" (r5), "r" (r6), "r" (r8)
        : "memory", "cc", "lr"
    );

    if (error_flag) {
        errno = (int)r0;
        return -1;
    }
    return r0;
}

/*
 * Performs a syscall with 8 arguments (uses stack helper).
 */

static inline __attribute__((overloadable, always_inline))
long syscall(long number, long arg1, long arg2, long arg3, long arg4, long arg5, long arg6, long arg7, long arg8)
{
    register long r0 __asm__("r0") = (long)arg1;
    register long r1 __asm__("r1") = (long)arg2;
    register long r2 __asm__("r2") = (long)arg3;
    register long r3 __asm__("r3") = (long)arg4;
    register long r12 __asm__("r12") = (long)number;
    register long r4 __asm__("r4") = (long)arg5;
    register long r5 __asm__("r5") = (long)arg6;
    register long r6 __asm__("r6") = (long)arg7;
    register long r8 __asm__("r8") = (long)arg8;
    long error_flag;

    __asm__ volatile (
        "bl _do_syscall\n\t"
        "mov %[err], #0\n\t"
        "it cs\n\t"
        "movcs %[err], #1\n\t"
        : "+r" (r0), [err] "=r" (error_flag)
        : "r" (r1), "r" (r2), "r" (r3), "r" (r12), "r" (r4), "r" (r5), "r" (r6), "r" (r8)
        : "memory", "cc", "lr"
    );

    if (error_flag) {
        errno = (int)r0;
        return -1;
    }
    return r0;
}

# endif // __APPLE__
#endif // __arm__

#endif // USELESS_SYSCALL_H
