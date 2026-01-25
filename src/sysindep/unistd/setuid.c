#include <unistd.h>
#include <sys/syscall.h>
int setuid(uid_t id) {
    return (int)syscall(SYS_setuid, id);
}
