#include <unistd.h>
#include <sys/syscall.h>

long fpathconf(int fd, int name) {
    return (long)syscall(SYS_fpathconf, fd, name);
}
