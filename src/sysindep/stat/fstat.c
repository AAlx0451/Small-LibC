#include <sys/stat.h>
#include <sys/syscall.h>
#include <unistd.h>

int fstat(int fd, struct stat *sb) {
    return syscall(SYS_fstat, fd, (long)sb);
}
