#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

int isatty(int fd) {
    struct termios t;
    return (ioctl(fd, TIOCGETA, &t) != -1);
}
