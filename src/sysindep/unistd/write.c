#include <stddef.h>
#include <unistd.h>
#include <sys/syscall.h>

#if !defined(SYS_write) && defined(__NR_write)
# define SYS_write __NR_write
#elif !defined(SYS_write)
# error Please include proper syscall.h. Current one should support armv7 linux and ios
#endif

ssize_t write(int fd, const void *buf, size_t count) {
    long ret;
    ret = syscall(SYS_write, fd, buf, count);
    if (ret < 0) {
        return -1;
    }
    return (ssize_t)ret;
}

