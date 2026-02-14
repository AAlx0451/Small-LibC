#include <sys/ioctl.h>
#include <termios.h>

int isatty(int fd) {
    struct termios t;
    return (ioctl(fd, TIOCGETA, &t) != -1);
}

