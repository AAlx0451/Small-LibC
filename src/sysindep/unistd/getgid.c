#include <unistd.h>
#include <sys/syscall.h>
gid_t getgid(void) {
    return (gid_t)syscall(SYS_getgid);
}
