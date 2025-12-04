#include <sys/types.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <unistd.h>
#include <sys/syscall.h>

pid_t wait4(pid_t pid, int *status, int options, struct rusage *rusage) {
    long ret;
    ret = syscall4(SYS_wait4, (long)pid, (long)status, (long)options, (long)rusage);
    if (ret < 0) {
        return -1;
    }
    return (pid_t)ret;
}
