#include <sys/syscall.h>
#include <unistd.h>

int chdir(const char *path) {
    return (int)syscall(SYS_chdir, (long)path);
}
