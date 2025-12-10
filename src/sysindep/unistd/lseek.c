#include <unistd.h>
#include <sys/syscall.h>

off_t lseek(int fd, off_t offset, int whence) {
    long res = syscall(SYS_lseek, fd, (long)(offset), (long)(offset >> 32), whence);
    return (off_t)res;
}
