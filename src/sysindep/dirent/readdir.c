#include <sys/syscall.h>
#include <unistd.h>
#include <dirent.h>
#include "dirent_private.h"

struct dirent *readdir(DIR *dir) {
    if (!dir) return NULL;

    for (;;) {
        if (dir->dd_loc >= dir->dd_len) {
            dir->dd_loc = 0;
            off_t basep = 0;
            int nread = syscall(SYS_getdirentries64, dir->dd_fd, (long)dir->dd_buf, DIR_BUFFER_SIZE, (long)&basep);
            
            if (nread <= 0) return NULL;
            dir->dd_len = nread;
        }

        struct dirent *dp = (struct dirent *)(dir->dd_buf + dir->dd_loc);
        
        if (dp->d_reclen == 0) return NULL;

        dir->dd_loc += dp->d_reclen;
        
        if (dp->d_ino != 0) return dp;
    }
}
