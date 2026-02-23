#include <sys/syscall.h>
#include <unistd.h>

long pathconf(const char *path, int name) {
    return (long)syscall(SYS_pathconf, (long)path, name);
}
