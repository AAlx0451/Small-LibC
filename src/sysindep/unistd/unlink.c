#include <sys/syscall.h>
#include <unistd.h>

int unlink(const char *path) {
    long res = syscall(SYS_unlink, (long)path);
    return (int)res;
}
