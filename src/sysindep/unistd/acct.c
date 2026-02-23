#include <sys/syscall.h>
#include <unistd.h>

int acct(const char *path) {
    return syscall(SYS_acct, (long)path);
}
