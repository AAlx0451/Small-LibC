#ifndef DIRENT_PRIVATE_H_
#define DIRENT_PRIVATE_H_

#define DIR_BUFFER_SIZE 4096

#include <dirent.h>

struct DIR {
    int           dd_fd;
    char          *dd_buf;
    long          dd_len;
    long          dd_loc;
    struct dirent dd_ent;
};

#endif
