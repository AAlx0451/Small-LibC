#include <unistd.h>
#include <sys/syscall.h>
gid_t getegid(void) {
    return (gid_t)syscall(SYS_getegid);
}
