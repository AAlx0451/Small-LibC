#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

#if defined(__GNUC__) || defined(__clang__)
#define unused __attribute__((unused))
#else
#define unused
#endif /* __GNUC__ || __clang__ */ 

int tcsendbreak(int fd, int unused duration) {
    if(ioctl(fd, TIOCSBRK, 0) == -1)
        return -1;
    usleep(400000);
    return ioctl(fd, TIOCCBRK, 0);
}
