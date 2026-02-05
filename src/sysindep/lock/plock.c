#include <sys/mman.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <errno.h>
#include <sys/lock.h>

int plock(int op) {
    switch (op) {
        case TXTLOCK:
            return syscall(SYS_mlockall, MCL_CURRENT);
        case DATLOCK:
            return syscall(SYS_mlockall, MCL_FUTURE);
        case PROCLOCK:
            return syscall(SYS_mlockall, MCL_CURRENT | MCL_FUTURE);
        case UNLOCK:
            return syscall(SYS_munlockall);
        default:
            errno = EINVAL;
            return -1;
    }
}

