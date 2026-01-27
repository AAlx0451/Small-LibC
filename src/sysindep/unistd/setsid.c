#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>

pid_t setsid(void) {
    return (pid_t)syscall(SYS_setsid);
}
