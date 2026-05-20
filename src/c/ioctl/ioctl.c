#include <stdarg.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/syscall.h>
#include <termios.h>
#include <unistd.h>

int ioctl(int fd, unsigned long request, ...)
{
    va_list ap;
    long arg;
    va_start(ap, request);
    arg = va_arg(ap, long);
    va_end(ap);
    return (int)syscall(SYS_ioctl, fd, (long)request, arg);
}
