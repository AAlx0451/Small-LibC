#include <unistd.h>
#include <sys/syscall.h>

int dup(int oldfd) {
    return (int)syscall(SYS_dup, oldfd);
}
