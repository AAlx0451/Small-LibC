#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

FILE *fdopen(int fildes, const char *mode) {
    unsigned int stdio_flags = 0;
    int fd_flags;
    fd_flags = fcntl(fildes, F_GETFL);
    if(fd_flags == -1) {
        return NULL;
    }

    if(strchr(mode, '+')) {
        stdio_flags |= __S_RW;
    } else if(*mode == 'r') {
        stdio_flags |= __S_RD;
    } else if(*mode == 'w' || *mode == 'a') {
        stdio_flags |= __S_WR;
    } else {
        errno = EINVAL;
        return NULL;
    }

    int access_mode = fd_flags & O_ACCMODE;

    if((stdio_flags & __S_RD) && (access_mode != O_RDONLY && access_mode != O_RDWR)) {
        errno = EINVAL;
        return NULL;
    }
    if((stdio_flags & __S_WR) && (access_mode != O_WRONLY && access_mode != O_RDWR)) {
        errno = EINVAL;
        return NULL;
    }

    FILE *f = malloc(sizeof(FILE));
    if(!f) {
        errno = ENOMEM;
        return NULL;
    }

    // Allocate +1 byte for ungetc reserve
    unsigned char *alloc_buf = malloc(BUFSIZ + 1);
    if(!alloc_buf) {
        free(f);
        errno = ENOMEM;
        return NULL;
    }

    f->_fd = fildes;

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

    if(*mode == 'a') {
        lseek(fildes, 0, SEEK_END);
    }

    __stdio_add_file(f);

    return f;
}
