#include <sys/syscall.h>
#include <unistd.h>

int setpgid(pid_t pid, pid_t pgid) {
    return (int)syscall(SYS_setpgid, pid, pgid);
}
