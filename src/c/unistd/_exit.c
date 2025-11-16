#include <unistd.h>
#include <sys/syscall.h>

__attribute__((noreturn)) void _exit(int status) {
    syscall(SYS_exit, status);
    __builtin_unreachable();
}
