#include <stdio.h>
#include <unistd.h>
#include <errno.h>

int remove(const char *pathname)
{
    if (unlink(pathname) == 0) {
        return 0;
    }

    if (errno == EPERM || errno == EISDIR) {
        return rmdir(pathname);
    }
    
    return -1;
}
