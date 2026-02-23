#include <errno.h>
#include <signal.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int waitid(idtype_t idtype, id_t id, siginfo_t *infop, int options) {
    return (int)syscall(SYS_waitid, (long)idtype, (long)id, (long)infop, (long)options, (long)NULL);
}
