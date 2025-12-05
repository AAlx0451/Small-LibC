#include <sys/types.h>
#include <sys/wait.h>
#include <sys/syscall.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>

int waitid(idtype_t idtype, id_t id, siginfo_t *infop, int options) {
    long ret;
    ret = syscall5(SYS_waitid, (long)idtype, (long)id, (long)infop, (long)options, (long)NULL);
    if (ret < 0) {
        return -1;
    }
    return (int)ret;
}
