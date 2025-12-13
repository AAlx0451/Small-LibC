#include <unistd.h>
#include <sys/syscall.h>

int dup2(int oldfd, int newfd) {
    return (int)syscall(SYS_dup2, (long)oldfd, (long)newfd);
}
