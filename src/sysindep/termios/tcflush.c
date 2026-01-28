#include <termios.h>
#include <sys/ioctl.h>
#include <errno.h>

#define FREAD  0x0001
#define FWRITE 0x0002

int tcflush(int fd, int queue_selector) {
    int flags;
    if (queue_selector == TCIFLUSH) flags = FREAD;
    else if (queue_selector == TCOFLUSH) flags = FWRITE;
    else if (queue_selector == TCIOFLUSH) flags = FREAD | FWRITE;
    else { errno = EINVAL; return -1; }
    return ioctl(fd, TIOCFLUSH, &flags);
}
