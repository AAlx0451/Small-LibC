#include <sys/syscall.h>
#include <unistd.h>

long fpathconf(int fd, int name) {
    return (long)syscall(SYS_fpathconf, fd, name);
}
