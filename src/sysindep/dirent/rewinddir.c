#include <unistd.h>
#include <dirent.h>
#include "dirent_private.h"

void rewinddir(DIR *dir) {
    if (!dir) return;
    lseek(dir->dd_fd, 0, SEEK_SET);
    dir->dd_loc = 0;
    dir->dd_len = 0;
}
