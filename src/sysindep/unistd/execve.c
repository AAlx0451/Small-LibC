#include <unistd.h>
#include <sys/syscall.h>

int execve(const char *pathname, char *const argv[], char *const envp[]) {
    return syscall(SYS_execve, (long)pathname, (long)argv, (long)envp);
}
