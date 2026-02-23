#include <sys/stat.h>
#include <sys/syscall.h>
#include <unistd.h>

int lstat(const char *path, struct stat *sb) {
    return syscall(SYS_lstat, (long)path, (long)sb);
}
