#include <stdarg.h>
#include <unistd.h>
#include <sys/syscall.h>

int ioctl(int fd, unsigned long request, ...) {
    long arg;
    va_list ap;

    va_start(ap, request);
    arg = va_arg(ap, long);
    va_end(ap);

    return (int)syscall(SYS_ioctl, fd, (long)request, arg);
}
