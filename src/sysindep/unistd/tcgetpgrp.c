#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>

pid_t tcgetpgrp(int fd) {
    int pgrp;
    
    if (ioctl(fd, TIOCGPGRP, &pgrp) < 0) {
        return (pid_t)-1;
    }

    return (pid_t)pgrp;
}
