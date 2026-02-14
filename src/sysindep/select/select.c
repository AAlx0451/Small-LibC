#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/select.h>

int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout) {
    return syscall(SYS_select, nfds, (long)readfds, (long)writefds, (long)exceptfds, (long)timeout);
}
