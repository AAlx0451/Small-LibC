#include <signal.h>
#include <unistd.h>
#include <sys/syscall.h>

int sigpending(sigset_t *set) {
    return (int)syscall(SYS_sigpending, (long)set);
}
