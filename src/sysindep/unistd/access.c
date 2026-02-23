#include <sys/syscall.h>
#include <unistd.h>

int access(const char *pathname, int mode) {
    return syscall(SYS_access, (long)pathname, mode);
}
