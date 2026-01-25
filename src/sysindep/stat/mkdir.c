#include <sys/types.h>
#include <sys/syscall.h>
#include <unistd.h>

int mkdir(const char *path, mode_t mode) 
{
    long ret = syscall(SYS_mkdir, (long)path, mode);

    if (ret < 0) 
    {
        return -1;
    }

    return 0;
}
