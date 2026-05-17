#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

int tcsendbreak(int fd, __unused int duration) {
    if(ioctl(fd, TIOCSBRK, 0) == -1)
        return -1;
    usleep(400000);
    return ioctl(fd, TIOCCBRK, 0);
}
