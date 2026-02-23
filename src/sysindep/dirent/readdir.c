#include "dirent_private.h"
#include <dirent.h>
#include <stddef.h> // offsetof
#include <string.h> // memcpy
#include <sys/syscall.h>
#include <unistd.h>

struct dirent *readdir(DIR *dir) {
    if(!dir) {
        return NULL;
    }

    for(;;) {
        if(dir->dd_loc >= dir->dd_len) {
            dir->dd_loc = 0;
            off_t basep = 0;
            int nread = (int)syscall(SYS_getdirentries64, dir->dd_fd, (long)(uintptr_t)dir->dd_buf, DIR_BUFFER_SIZE, (long)(uintptr_t)&basep);

            if(nread <= 0) {
                return NULL;
            }
            dir->dd_len = nread;
        }

        char *current_record = dir->dd_buf + dir->dd_loc;

        uint16_t reclen;
        memcpy(&reclen, current_record + offsetof(struct dirent, d_reclen), sizeof(reclen));

        if(reclen == 0) {
            return NULL;
        }

        size_t copy_size = reclen;
        if(copy_size > sizeof(struct dirent)) {
            copy_size = sizeof(struct dirent);
        }
        memcpy(&dir->dd_ent, current_record, copy_size);
        dir->dd_loc += reclen;

        if(dir->dd_ent.d_ino != 0) {
            return &dir->dd_ent;
        }
    }
}
