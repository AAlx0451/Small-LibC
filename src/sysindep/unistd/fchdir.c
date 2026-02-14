#include <unistd.h>
#include <sys/syscall.h>

int fchdir(int fd) {
    return (int)syscall(SYS_fchdir, fd);
}
