#include <unistd.h>
#include <sys/syscall.h>
uid_t geteuid(void) {
    return (uid_t)syscall(SYS_geteuid);
}
