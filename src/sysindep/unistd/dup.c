#include <sys/syscall.h>
#include <unistd.h>

int dup(int oldfd) {
    return (int)syscall(SYS_dup, oldfd);
}
