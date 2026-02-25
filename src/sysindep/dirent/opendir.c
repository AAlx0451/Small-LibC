#include "dirent_private.h"
#include <dirent.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

DIR *opendir(const char *name) {
    DIR *dir = (DIR *)malloc(sizeof(DIR));
    int fd = open(name, O_RDONLY | O_DIRECTORY);
    if(fd == -1)
        return NULL;

    if(!dir) {
        close(fd);
        return NULL;
    }

    dir->dd_buf = (char *)malloc(DIR_BUFFER_SIZE);
    if(!dir->dd_buf) {
        free(dir);
        close(fd);
        return NULL;
    }

    dir->dd_fd = fd;
    dir->dd_len = 0;
    dir->dd_loc = 0;

    return dir;
}
