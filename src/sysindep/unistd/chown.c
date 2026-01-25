#include <unistd.h>
#include <sys/syscall.h>

int chown(const char *path, uid_t owner, gid_t group) {
    return (int)syscall(SYS_chown, (long)path, owner, group);
}
