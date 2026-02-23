#include <sys/syscall.h>
#include <unistd.h>
int setuid(uid_t id) {
    return (int)syscall(SYS_setuid, (long)id);
}
