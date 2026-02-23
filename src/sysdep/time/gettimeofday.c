#if defined(__APPLE__) && defined(__arm__)

#include <errno.h>
#include <sys/syscall.h>
#include <sys/time.h>

#if defined(__GNUC__) || defined(__clang__)
#define _unused __attribute__((unused))
#else
#define _unused
#endif /* __GNUC__ || __clang__ */

int gettimeofday(struct timeval *tp, void *_unused tzp) {
    register long r12_syscall __asm__("r12") = SYS_gettimeofday;
    long sec, usec;
    long err;
    __asm__ volatile(
        "svc #0x80\n\t"
        "bcc 1f\n\t"
        "mov %[err], r0\n\t"
        "mov %[sec], #0\n\t"
        "mov %[usec], #0\n\t"
        "b 2f\n\t"
        "1:\n\t"
        "mov %[err], #0\n\t"
        "mov %[sec], r0\n\t"
        "mov %[usec], r1\n\t"
        "2:\n\t"
        : [sec] "=r"(sec),
          [usec] "=r"(usec),
          [err] "=r"(err)
        : "r"(r12_syscall)
        : "r0", "r1", "cc", "memory");

    if(err != 0) {
        errno = (int)err;
        return -1;
    }

    if(tp) {
        tp->tv_sec = (time_t)sec;
        tp->tv_usec = (suseconds_t)usec;
    }

    return 0;
}

#else

#error

#endif
