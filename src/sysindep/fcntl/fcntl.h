#include <unistd.h>
#include <sys/syscall.h>
#include <stdarg.h>

int fcntl(int fd, int cmd, ...) {
    va_list args;
    long arg;

    va_start(args, cmd);
    arg = va_arg(args, long);
    va_end(args);

    return syscall(SYS_fcntl, fd, cmd, arg);
}
