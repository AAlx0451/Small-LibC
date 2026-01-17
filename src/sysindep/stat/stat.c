#include <unistd.h>
#include <sys/syscall.h>
#include <sys/stat.h>

int stat(const char *path, struct stat *sb) {
    return syscall(SYS_stat, (long)path, (long)sb);
}

