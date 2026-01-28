#include <termios.h>
#include <sys/ioctl.h>
#include <unistd.h>

int tcsendbreak(int fd, int duration) {
    if (ioctl(fd, TIOCSBRK, 0) == -1) return -1;
    usleep(400000);
    return ioctl(fd, TIOCCBRK, 0);
}
