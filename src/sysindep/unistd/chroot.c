#include <unistd.h>
#include <sys/syscall.h>

int chroot(const char *path) {
    return syscall(SYS_chroot, (long)path);
}
