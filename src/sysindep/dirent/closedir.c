#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include "dirent_private.h"

int closedir(DIR *dir) {
    if (!dir) return -1;
    close(dir->dd_fd);
    free(dir->dd_buf);
    free(dir);
    return 0;
}
