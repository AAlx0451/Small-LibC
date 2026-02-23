#include <sys/syscall.h>
#include <unistd.h>

int fchown(int fd, uid_t owner, gid_t group) {
    return (int)syscall(SYS_fchown, fd, owner, group);
}
