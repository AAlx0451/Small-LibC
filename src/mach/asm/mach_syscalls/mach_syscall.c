#include <mach/mach_syscalls.h>
#include <stdarg.h>
#include <unistd.h>

#define REG(name, val) register long name __asm__(#name) = (long)(val)

/*
 * simple helper
 * aligns stack by 32 bytes (pushing 8 regs) and marks stack changes for CFI.
 * This accommodates up to 9 arguments (5 on the stack: r4, r5, r6, r8, r10).
 * r11, r12, and lr act as padding to maintain 16-byte alignment.
 */
static __naked __noinline __used void do_mach_syscall(void)
{
    __asm__ volatile(".cfi_startproc\n\t"
                     "push {r4, r5, r6, r8, r10, r11, r12, lr}\n\t" // r7 is a frame pointer
                     ".cfi_def_cfa_offset 32\n\t"
                     ".cfi_offset r4, -32\n\t"
                     ".cfi_offset r5, -28\n\t"
                     ".cfi_offset r6, -24\n\t"
                     ".cfi_offset r8, -20\n\t"
                     ".cfi_offset r10, -16\n\t"
                     ".cfi_offset r11, -12\n\t"
                     ".cfi_offset r12, -8\n\t"
                     ".cfi_offset lr, -4\n\t"
                     "svc #0x80\n\t"
                     "pop {r4, r5, r6, r8, r10, r11, r12, lr}\n\t"
                     ".cfi_def_cfa_offset 0\n\t"
                     "bx lr\n\t"
                     ".cfi_endproc\n\t");
}

// we don't add r4-r6,r8 to clobbers, because we bind
// args to register and compiler will generate correct prologue
// r0-r3,r12 are caller-saved, so no clobbers too
// "lr" in clobbers because we now use 'bl' instruction for >4 args
// mach traps do not return errno, so we just return r0 directly
#define DEFINE_MACH_SYSCALL(n, args, regs, asm_code, ...)                                          \
    long _mach_syscall##n args                                                                     \
    {                                                                                              \
        regs                                                                                       \
                                                                                                   \
            __asm__ volatile(asm_code : "+r"(r0) : __VA_ARGS__ : "memory", "lr");                  \
                                                                                                   \
        return r0;                                                                                 \
    }

DEFINE_MACH_SYSCALL(0, (long number), REG(r0, 0); REG(r12, number);, "svc #0x80\n\t", "r"(r12))

DEFINE_MACH_SYSCALL(1, (long number, long arg1), REG(r0, arg1); REG(r12, number);
                    , "svc #0x80\n\t", "r"(r12))

DEFINE_MACH_SYSCALL(2, (long number, long arg1, long arg2), REG(r0, arg1); REG(r1, arg2);
                    REG(r12, number);
                    , "svc #0x80\n\t", "r"(r1), "r"(r12))

DEFINE_MACH_SYSCALL(3, (long number, long arg1, long arg2, long arg3), REG(r0, arg1); REG(r1, arg2);
                    REG(r2, arg3);
                    REG(r12, number);
                    , "svc #0x80\n\t", "r"(r1), "r"(r2), "r"(r12))

DEFINE_MACH_SYSCALL(4, (long number, long arg1, long arg2, long arg3, long arg4), REG(r0, arg1);
                    REG(r1, arg2);
                    REG(r2, arg3);
                    REG(r3, arg4);
                    REG(r12, number);
                    , "svc #0x80\n\t", "r"(r1), "r"(r2), "r"(r3), "r"(r12))

// for 5+ args we use _do_mach_syscall
// we always fill r4, r5, r6, r8 to for alignment

DEFINE_MACH_SYSCALL(5,
                    (long number, long arg1, long arg2, long arg3, long arg4, long arg5),
                    REG(r0, arg1);
                    REG(r1, arg2);
                    REG(r2, arg3);
                    REG(r3, arg4);
                    REG(r12, number);
                    REG(r4, arg5);
                    REG(r5, 0);
                    REG(r6, 0);
                    REG(r8, 0);
                    , // r5, r6, r8 are for padding
                    "bl _do_mach_syscall\n\t",
                    "r"(r1),
                    "r"(r2),
                    "r"(r3),
                    "r"(r12),
                    "r"(r4),
                    "r"(r5),
                    "r"(r6),
                    "r"(r8))

DEFINE_MACH_SYSCALL(6,
                    (long number, long arg1, long arg2, long arg3, long arg4, long arg5, long arg6),
                    REG(r0, arg1);
                    REG(r1, arg2);
                    REG(r2, arg3);
                    REG(r3, arg4);
                    REG(r12, number);
                    REG(r4, arg5);
                    REG(r5, arg6);
                    REG(r6, 0);
                    REG(r8, 0);
                    , // r6, r8 are for padding
                    "bl _do_mach_syscall\n\t",
                    "r"(r1),
                    "r"(r2),
                    "r"(r3),
                    "r"(r12),
                    "r"(r4),
                    "r"(r5),
                    "r"(r6),
                    "r"(r8))

DEFINE_MACH_SYSCALL(
    7,
    (long number, long arg1, long arg2, long arg3, long arg4, long arg5, long arg6, long arg7),
    REG(r0, arg1);
    REG(r1, arg2);
    REG(r2, arg3);
    REG(r3, arg4);
    REG(r12, number);
    REG(r4, arg5);
    REG(r5, arg6);
    REG(r6, arg7);
    REG(r8, 0);
    , // r8 is for stack padding
    "bl _do_mach_syscall\n\t",
    "r"(r1),
    "r"(r2),
    "r"(r3),
    "r"(r12),
    "r"(r4),
    "r"(r5),
    "r"(r6),
    "r"(r8))

DEFINE_MACH_SYSCALL(8,
                    (long number,
                     long arg1,
                     long arg2,
                     long arg3,
                     long arg4,
                     long arg5,
                     long arg6,
                     long arg7,
                     long arg8),
                    REG(r0, arg1);
                    REG(r1, arg2);
                    REG(r2, arg3);
                    REG(r3, arg4);
                    REG(r12, number);
                    REG(r4, arg5);
                    REG(r5, arg6);
                    REG(r6, arg7);
                    REG(r8, arg8);
                    ,
                    "bl _do_mach_syscall\n\t",
                    "r"(r1),
                    "r"(r2),
                    "r"(r3),
                    "r"(r12),
                    "r"(r4),
                    "r"(r5),
                    "r"(r6),
                    "r"(r8))

DEFINE_MACH_SYSCALL(9,
                    (long number,
                     long arg1,
                     long arg2,
                     long arg3,
                     long arg4,
                     long arg5,
                     long arg6,
                     long arg7,
                     long arg8,
                     long arg9),
                    REG(r0, arg1);
                    REG(r1, arg2);
                    REG(r2, arg3);
                    REG(r3, arg4);
                    REG(r12, number);
                    REG(r4, arg5);
                    REG(r5, arg6);
                    REG(r6, arg7);
                    REG(r8, arg8);
                    REG(r10, arg9);
                    ,
                    "bl _do_mach_syscall\n\t",
                    "r"(r1),
                    "r"(r2),
                    "r"(r3),
                    "r"(r12),
                    "r"(r4),
                    "r"(r5),
                    "r"(r6),
                    "r"(r8),
                    "r"(r10))
