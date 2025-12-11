#include <unistd.h>
#include <sys/syscall.h>
#include <sys/ioctl.h>
#include <errno.h>

int isatty(int fd)
{
    struct winsize ws;

    if ((syscall(SYS_ioctl, fd, TIOCGWINSZ, (long)&ws)) < 0) {
        return 0;
    }

    return 1;
}
