#include <errno.h>
#include <sys/ioctl.h>
#include <termios.h>

int tcsetattr(int fd, int opt, const struct termios *t) {
    unsigned long request;
    switch(opt) {
    case TCSANOW:
        request = TIOCSETA;
        break;
    case TCSADRAIN:
        request = TIOCSETAW;
        break;
    case TCSAFLUSH:
        request = TIOCSETAF;
        break;
    default:
        errno = EINVAL;
        return -1;
    }
    return ioctl(fd, request, (void *)t);
}
