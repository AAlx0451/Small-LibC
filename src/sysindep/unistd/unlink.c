#include <unistd.h>
#include <sys/syscall.h>

int unlink(const char *path) {
    long res = syscall(SYS_unlink, (long)path);
    return (int)res;
}
