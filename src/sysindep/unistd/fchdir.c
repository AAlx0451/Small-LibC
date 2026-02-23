#include <sys/syscall.h>
#include <unistd.h>

int fchdir(int fd) {
    return (int)syscall(SYS_fchdir, fd);
}
