#include <termios.h>
#include <sys/ioctl.h>
#include <errno.h>

int tcflow(int fd, int action) {
    switch (action) {
        case TCOOFF: return ioctl(fd, TIOCSTOP, 0);
        case TCOON:  return ioctl(fd, TIOCSTART, 0);
        case TCIOFF:
        case TCION:
            errno = EINVAL; return -1;
        default: errno = EINVAL; return -1;
    }
}
