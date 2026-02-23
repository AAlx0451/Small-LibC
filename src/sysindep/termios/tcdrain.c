#include <sys/ioctl.h>
#include <termios.h>

int tcdrain(int fd) {
    return ioctl(fd, TIOCDRAIN, 0);
}
