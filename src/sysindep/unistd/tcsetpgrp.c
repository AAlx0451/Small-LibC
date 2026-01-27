#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>

int tcsetpgrp(int fd, pid_t pgrp) {
    int s_pgrp = (int)pgrp;
    return ioctl(fd, TIOCSPGRP, &s_pgrp);
}
