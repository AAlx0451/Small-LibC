#include <sys/syscall.h>
#include <unistd.h>

int mkfifo(const char *path, mode_t mode) {
    return syscall(SYS_mkfifo, (long)path, mode);
}
