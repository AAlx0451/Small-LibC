#if defined(__APPLE__) && defined(__arm__)

#include <unistd.h>
#include <sys/syscall.h>
#include <errno.h>
#include <sys/types.h>

#if !defined(SYS_fork) && defined(__NR_fork)
# define SYS_fork __NR_fork
#elif !defined(SYS_fork)
#error "See syscall.h"
#endif

pid_t fork(void) {
    register long r0 __asm__("r0");
    register long r1 __asm__("r1");
    register long r12 __asm__("r12") = SYS_fork;
    long error_flag;

    __asm__ volatile (
        "svc #0x80\n\t"
        "mov %[err], #0\n\t"
        "movcs %[err], #1\n\t"
        : "=r" (r0), "=r" (r1), [err] "=r" (error_flag)
        : "r" (r12)
        : "memory", "cc"
    );

    if (error_flag) {
        errno = (int)r0;
        return -1;
    }

    if (r1 != 0) {
        return 0;
    }

    return (pid_t)r0;
}

#else

# error "Sorry, fork for non-iOS not implemented yet. Yes, it's machine dependent..."

#endif
