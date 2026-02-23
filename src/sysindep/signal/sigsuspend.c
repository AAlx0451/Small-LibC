#include <errno.h>
#include <signal.h>
#include <sys/syscall.h>
#include <unistd.h>

int sigsuspend(const sigset_t *sigmask) {
    if(!sigmask) {
        errno = EFAULT;
        return -1;
    }

    return (int)syscall(SYS_sigsuspend, *sigmask);
}
