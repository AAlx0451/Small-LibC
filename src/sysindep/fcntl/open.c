#include <sys/types.h>
#include <fcntl.h>
#include <stdarg.h>
#include <unistd.h>
#include <sys/syscall.h>

int open(const char *pathname, int flags, ...) {
    long ret;
    if (flags & O_CREAT) {
        va_list args;
        mode_t mode;
        va_start(args, flags);
        mode = (mode_t)va_arg(args, int);
        va_end(args);
        return (int)(syscall(SYS_open, (long)pathname, flags, mode));
    } else {
        return (int)(syscall(SYS_open, (long)pathname, flags));
    }
}
