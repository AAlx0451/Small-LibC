#include "dirent_private.h"
#include <dirent.h>
#include <stdlib.h>
#include <unistd.h>

int closedir(DIR *dir) {
    if(!dir)
        return -1;
    close(dir->dd_fd);
    free(dir->dd_buf);
    free(dir);
    return 0;
}
