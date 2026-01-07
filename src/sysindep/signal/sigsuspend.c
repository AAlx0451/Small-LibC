#include <signal.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <errno.h>

int sigsuspend(const sigset_t *sigmask)
{
    if (!sigmask) {
        errno = EFAULT; 
        return -1;
    }

    return (int)syscall(SYS_sigsuspend, *sigmask);
}
