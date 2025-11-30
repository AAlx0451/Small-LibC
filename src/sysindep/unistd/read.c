#include <stddef.h>
#include <unistd.h>
#include <sys/syscall.h>

#if !defined(SYS_read) && defined(__NR_read)
# define SYS_read __NR_read
#elif !defined(SYS_read)
# error Please include proper syscall.h. Current one should support armv7 ios
#endif

ssize_t read(int fd, void *buf, size_t count) {
    long ret;
    ret = syscall3(SYS_read, fd, (long)buf, count);
    if (ret < 0) {
        return -1;
    }
    return (ssize_t)ret;
}

