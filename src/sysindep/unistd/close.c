#include <sys/syscall.h>
#include <unistd.h>

int close(int fd) {
    return syscall(SYS_close, (long)fd);
}
