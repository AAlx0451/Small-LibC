#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>

pid_t getppid(void) {
    return (pid_t)syscall(SYS_getppid);
}
