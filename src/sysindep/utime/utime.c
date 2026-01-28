#include <utime.h>
#include <sys/syscall.h>
#include <stddef.h>
#include <sys/time.h>
#include <unistd.h>

int utime(const char *path, const struct utimbuf *times) {
    if (times != NULL) {
        struct timeval tv[2];
        
        tv[0].tv_sec = times->actime;
        tv[0].tv_usec = 0;
        
        tv[1].tv_sec = times->modtime;
        tv[1].tv_usec = 0;
        
        return syscall(SYS_utimes, (long)path, (long)tv);
    }
    
    return syscall(SYS_utimes, (long)path, (long)NULL);
}
