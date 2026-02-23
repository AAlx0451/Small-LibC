#include <stdio.h>
#include <sys/syscall.h>
#include <unistd.h>

int rename(const char *oldpath, const char *newpath) {
    long result = syscall(SYS_rename, (long)oldpath, (long)newpath);
    if(result == -1) {
        return -1;
    }
    return 0;
}
