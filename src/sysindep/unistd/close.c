#include <unistd.h>
#include <sys/syscall.h>

#if !defined(SYS_close) && defined(__NR_close)
# define SYS_close __NR_close
#elif !defined(SYS_close)
# error "Please include a proper syscall.h for your architecture. See sys/syscall.h"
#endif

int close(int fd) {
    long ret;
    ret = syscall1(SYS_close, (long)fd);
    if (ret < 0) {
        return -1;
    }
    return 0;
}
