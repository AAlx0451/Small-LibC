#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>

FILE *fopen(const char *pathname, const char *mode) {
    int open_flags = 0;
    int stdio_flags = 0;

    if (strchr(mode, '+')) {
        open_flags |= O_RDWR;
        stdio_flags |= __S_RW;
    } else if (*mode == 'r') {
        open_flags |= O_RDONLY;
        stdio_flags |= __S_RD;
    } else {
        open_flags |= O_WRONLY;
        stdio_flags |= __S_WR;
    }

    if (*mode == 'w') {
        open_flags |= O_CREAT | O_TRUNC;
    } else if (*mode == 'a') {
        open_flags |= O_CREAT | O_APPEND;
    } else if (*mode == 'r' && !(stdio_flags & __S_RW)) {
        // 'r' or 'rb'
    } else if (strchr(mode, '+')) {
         if (*mode != 'r') {
             open_flags |= O_CREAT;
         }
    } else {
        errno = EINVAL;
        return NULL;
    }

    int fd = open(pathname, open_flags, 0666);
    if (fd < 0) {
        return NULL;
    }

    FILE *f = malloc(sizeof(FILE));
    if (!f) {
        close(fd);
        errno = ENOMEM;
        return NULL;
    }

    unsigned char *buf = malloc(BUFSIZ);
    if (!buf) {
        close(fd);
        free(f);
        errno = ENOMEM;
        return NULL;
    }

    f->_fd = fd;
    f->_flags = stdio_flags;
    f->_base = buf;
    f->_ptr = buf;
    f->_bsize = BUFSIZ;
    f->_lock = 0;
    f->_next = NULL;

    if (stdio_flags & __S_RD) {
        f->_cnt = 0;
    } else {
        f->_cnt = BUFSIZ;
    }
    
    __stdio_add_file(f);

    return f;
}
