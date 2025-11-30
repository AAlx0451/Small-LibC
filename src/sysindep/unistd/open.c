#include <sys/types.h>
#include <fcntl.h>
#include <stdarg.h>
#include <unistd.h>
#include <sys/syscall.h>

#if !defined(SYS_open) && defined(__NR_open)
# define SYS_open __NR_open
#elif !defined(SYS_open)
# error "Please include a proper syscall.h for your architecture. See sys/syscall.h"
#endif

int open(const char *pathname, int flags, ...) {
    long ret;
    if (flags & O_CREAT) {
        va_list args;
        mode_t mode;
        va_start(args, flags);
        mode = va_arg(args, mode_t);
        va_end(args);
        ret = syscall3(SYS_open, (long)pathname, flags, mode);
    } else {
        ret = syscall2(SYS_open, (long)pathname, flags);
    }

    if (ret < 0) {
        return -1;
    }

    return (int)ret;
}
