#include <sys/stat.h>
#include <unistd.h>
#include <sys/syscall.h>

int lstat(const char *path, struct stat *sb) {
    return syscall(SYS_lstat, (long)path, (long)sb);
}
