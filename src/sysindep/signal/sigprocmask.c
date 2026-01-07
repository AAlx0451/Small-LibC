#include <unistd.h>
#include <sys/syscall.h>
#include <signal.h>

int sigprocmask(int how, const sigset_t *set, sigset_t *oldset)
{
    return (int)syscall(SYS_sigprocmask, how, (long)set, (long)oldset);
}
