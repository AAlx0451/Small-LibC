#include <termios.h>
#include <sys/ioctl.h>

int tcgetattr(int fd, struct termios *t) {
    return ioctl(fd, TIOCGETA, t);
}
