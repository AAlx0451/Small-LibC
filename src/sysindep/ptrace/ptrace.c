#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>

int ptrace(int request, pid_t pid, caddr_t addr, int data) {
    return syscall(SYS_ptrace, request, pid, (long)addr, data);
}
