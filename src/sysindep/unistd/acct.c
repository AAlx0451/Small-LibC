#include <unistd.h>
#include <sys/syscall.h>

int acct(const char *path) {
    return syscall(SYS_acct, (long)path);
}
