#include <sys/select.h>
#include <sys/syscall.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout) {
    return syscall(SYS_select, nfds, (long)readfds, (long)writefds, (long)exceptfds, (long)timeout);
}
