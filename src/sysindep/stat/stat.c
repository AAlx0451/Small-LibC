#include <sys/stat.h>
#include <sys/syscall.h>
#include <unistd.h>

int stat(const char *path, struct stat *sb) {
    return syscall(SYS_stat, (long)path, (long)sb);
}
