#include <stddef.h>
#include <sys/syscall.h>
#include <unistd.h>

ssize_t read(int fd, void *buf, size_t count) {
    return (ssize_t)syscall(SYS_read, fd, (long)buf, (long)count);
}
