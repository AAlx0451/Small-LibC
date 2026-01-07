#include <unistd.h>
#include <sys/syscall.h>

int chdir(const char *path) {
    return (int)syscall(SYS_chdir, (long)path);
}
