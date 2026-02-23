#include <sys/syscall.h>
#include <sys/time.h>
#include <unistd.h>

int utimes(const char *path, const struct timeval tv[2]) {
    return syscall(SYS_utimes, (long)path, (long)tv);
}
