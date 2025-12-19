#include <unistd.h>
#include <sys/syscall.h>

int access(const char *pathname, int mode) {
    return syscall(SYS_access, (long)pathname, mode);
}
