#include <unistd.h>
#include <sys/syscall.h>
int seteuid(uid_t euid) {
    return (int)syscall(SYS_seteuid, euid);
}
