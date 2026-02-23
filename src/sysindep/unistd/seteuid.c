#include <sys/syscall.h>
#include <unistd.h>
int seteuid(uid_t euid) {
    return (int)syscall(SYS_seteuid, euid);
}
