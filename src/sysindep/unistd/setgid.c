#include <unistd.h>
#include <sys/syscall.h>
int setgid(gid_t id) {
    return (int)syscall(SYS_setgid, id);
}
