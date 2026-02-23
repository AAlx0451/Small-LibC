#include <signal.h>
#include <sys/syscall.h>
#include <unistd.h>

int sigpending(sigset_t *set) {
    return (int)syscall(SYS_sigpending, (long)set);
}
