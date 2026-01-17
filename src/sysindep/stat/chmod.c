#include <sys/syscall.h>
#include <unistd.h>

int chmod(const char *path, mode_t mode) {
    return syscall(SYS_chmod, (long)path, mode);
}
