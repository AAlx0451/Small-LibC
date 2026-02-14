#include <unistd.h>
#include <sys/syscall.h>

int close(int fd) {
    return syscall(SYS_close, (long)fd);
}
