#include <sys/syscall.h>
#include <unistd.h>

__noreturn void _exit(int status) {
    syscall(SYS_exit, status);
    __unreachable();
}
