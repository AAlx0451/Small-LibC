#include <termios.h>
#include <sys/ioctl.h>

int tcdrain(int fd) {
    return ioctl(fd, TIOCDRAIN, 0);
}
