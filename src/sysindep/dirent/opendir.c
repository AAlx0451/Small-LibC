#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include "dirent_private.h"

DIR *opendir(const char *name) {
    int fd = open(name, O_RDONLY | O_DIRECTORY);
    if (fd == -1) return NULL;

    DIR *dir = malloc(sizeof(DIR));
    if (!dir) {
        close(fd);
        return NULL;
    }

    dir->dd_buf = malloc(DIR_BUFFER_SIZE);
    if (!dir->dd_buf) {
        free(dir);
        close(fd);
        return NULL;
    }

    dir->dd_fd = fd;
    dir->dd_len = 0;
    dir->dd_loc = 0;

    return dir;
}
