#include <errno.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>

pid_t fork(void)
{
    register long r0 __asm__("r0");
    register long r1 __asm__("r1");
    register long r12 __asm__("r12") = SYS_fork;
    long error_flag;

    __asm__ volatile("svc #0x80\n\t"
                     "mov %[err], #0\n\t"
                     "it cs\n\t"
                     "movcs %[err], #1\n\t"
                     : "=r"(r0), "=r"(r1), [err] "=r"(error_flag)
                     : "r"(r12)
                     : "memory", "cc");

    if (error_flag) {
        errno = (int)r0;
        return -1;
    }

    if (r1 != 0) {
        return 0;
    }

    return (pid_t)r0;
}
