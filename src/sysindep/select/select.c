#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/select.h>

int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout) {
    long ret;
    ret = syscall5(SYS_select, (long)nfds, (long)readfds, (long)writefds, (long)exceptfds, (long)timeout);
    if (ret < 0) {
        return -1;
    }
    return (int)ret;
}
