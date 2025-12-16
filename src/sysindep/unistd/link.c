#include <unistd.h>
#include <sys/syscall.h>

int link(const char *oldpath, const char *newpath) {
    long res = syscall(SYS_link, (long)oldpath, (long)newpath);
    return (int)res;
}
