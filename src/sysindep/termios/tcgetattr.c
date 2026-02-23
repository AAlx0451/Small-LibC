#include <sys/ioctl.h>
#include <termios.h>

int tcgetattr(int fd, struct termios *t) {
    return ioctl(fd, TIOCGETA, t);
}
