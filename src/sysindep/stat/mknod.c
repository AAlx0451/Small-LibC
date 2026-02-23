#include <sys/stat.h>
#include <sys/syscall.h>
#include <unistd.h>

int mknod(const char *path, mode_t mode, dev_t dev) {
    return syscall(SYS_mknod, (long)path, mode, dev);
}
