#include <sys/syscall.h>
#include <unistd.h>
int setgid(gid_t id) {
    return (int)syscall(SYS_setgid, id);
}
