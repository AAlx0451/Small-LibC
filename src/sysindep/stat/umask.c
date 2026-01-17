#include <sys/syscall.h>
#include <unistd.h>

mode_t umask(mode_t mask) {
    return (mode_t)syscall(SYS_umask, mask);
}
