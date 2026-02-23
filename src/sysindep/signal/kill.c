#include <signal.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>

int kill(pid_t pid, int sig) {
    return (int)syscall(SYS_kill, pid, sig);
}
