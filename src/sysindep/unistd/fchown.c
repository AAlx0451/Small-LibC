#include <unistd.h>
#include <sys/syscall.h>

int fchown(int fd, uid_t owner, gid_t group) {
    return (int)syscall(SYS_fchown, fd, owner, group);
}
