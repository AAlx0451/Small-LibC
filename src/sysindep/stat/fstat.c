#include <unistd.h>
#include <sys/syscall.h>
#include <sys/stat.h>

int fstat(int fd, struct stat *sb) {
    return syscall(SYS_fstat, fd, (long)sb);
}

