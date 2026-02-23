#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

FILE *freopen(const char *pathname, const char *mode, FILE *stream) {
    int open_flags = 0;
    unsigned int stdio_flags = 0;
    int fd;
    unsigned int preserve_flags;

    if(stream == NULL) {
        return NULL;
    }

    _spin_lock(&stream->_lock);

    /* flush existing data */
    __stdio_flush_impl(stream);

    /* close the old file descriptor */
    /* even if open fails later, the original stream is closed */
    if(stream->_fd >= 0) {
        close(stream->_fd);
    }
    stream->_fd = -1;
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
        /* 'r' or 'rb' - nothing extra */
    } else if(strchr(mode, '+')) {
        if(*mode != 'r') {
            open_flags |= O_CREAT;
        }
    } else {
        /* Invalid mode */
        _spin_unlock(&stream->_lock);
        fclose(stream); /* cleanup resources */
        errno = EINVAL;
        return NULL;
    }

    /* open new file descriptor */
    fd = open(pathname, open_flags, 0666);
    if(fd < 0) {
        _spin_unlock(&stream->_lock);
        fclose(stream); /* fclose will get the lock */
        return NULL;
    }

    /* reinitialize the stream state */
    stream->_fd = fd;

    /* preserve memory management flags */
    preserve_flags = stream->_flags & (__S_FREEBUF | __S_RESERVE | __S_STR);
    stream->_flags = stdio_flags | preserve_flags;

    /* reset buffer pointers */
    stream->_ptr = stream->_base;

    if(stdio_flags & __S_RD) {
        stream->_cnt = 0;
    } else {
        stream->_cnt = stream->_bsize;
    }

    /* release lock and return */
    _spin_unlock(&stream->_lock);

    return stream;
}
