#include <stddef.h>
#include <sys/syscall.h>
#include <unistd.h>

ssize_t write(int fd, const void *buf, size_t count) {
    long ret = syscall(SYS_write, fd, (long)buf, count);
    if(ret < 0) {
        return -1;
    }
    return (ssize_t)ret;
}
