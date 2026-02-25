#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

FILE *fopen(const char *pathname, const char *mode) {
    int open_flags = 0, fd;
    unsigned int stdio_flags = 0;
    unsigned char *alloc_buf;
    FILE *f;

    if(strchr(mode, '+')) {
        open_flags |= O_RDWR;
        stdio_flags |= __S_RW;
    } else if(*mode == 'r') {
        open_flags |= O_RDONLY;
        stdio_flags |= __S_RD;
    } else {
        open_flags |= O_WRONLY;
        stdio_flags |= __S_WR;
    }

    if(*mode == 'w') {
        open_flags |= O_CREAT | O_TRUNC;
    } else if(*mode == 'a') {
        open_flags |= O_CREAT | O_APPEND;
    } else if(*mode == 'r' && !(stdio_flags & __S_RW)) {
        // 'r' or 'rb'
    } else if(strchr(mode, '+')) {
        if(*mode != 'r') {
            open_flags |= O_CREAT;
        }
    } else {
        errno = EINVAL;
        return NULL;
    }

    fd = open(pathname, open_flags, 0666);
    if(fd < 0) {
        return NULL;
    }

    f = (FILE *)malloc(sizeof(FILE));
    if(!f) {
        close(fd);
        errno = ENOMEM;
        return NULL;
    }

    // Allocate +1 byte for ungetc reserve.
    alloc_buf = (unsigned char *)malloc(BUFSIZ + 1);
    if(!alloc_buf) {
        close(fd);
        free(f);
        errno = ENOMEM;
        return NULL;
    }

    f->_fd = fd;

    // Set __S_FREEBUF since we allocated buf
    // Set __S_RESERVE to indicate offset base
    f->_flags = stdio_flags | __S_FREEBUF | __S_RESERVE;

    // Offset base by 1 to allow ungetc at start of stream
    f->_base = alloc_buf + 1;
    f->_ptr = f->_base;

    f->_bsize = BUFSIZ;
    f->_lock = 0;
    f->_next = NULL;

    if(stdio_flags & __S_RD) {
        f->_cnt = 0;
    } else {
        f->_cnt = BUFSIZ;
    }

    __stdio_add_file(f);

    return f;
}
