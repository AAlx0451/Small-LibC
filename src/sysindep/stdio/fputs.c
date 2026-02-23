#include <stdio.h>
#include <string.h> // for strlen()
#include <unistd.h> // for write()

int fputs(const char *s, FILE *stream) {
    size_t len;
    int ret = 0;

    if(s == NULL || stream == NULL) {
        return EOF;
    }
    len = strlen(s);
    if(len == 0) {
        return 0;
    }

    _spin_lock(&stream->_lock);

    /* Ensure stream is in write mode */
    if(stream->_flags & __S_RD) {
        stream->_flags &= ~__S_RD;
        stream->_flags |= __S_WR;
        stream->_cnt = stream->_bsize;
        stream->_ptr = stream->_base;
    } else if(!(stream->_flags & __S_WR)) {
        stream->_flags |= __S_WR;
        if(stream->_cnt == 0 && stream->_ptr == stream->_base) {
            stream->_cnt = stream->_bsize;
        }
    }

    /* If stream is unbuffered, use a single syscall for the whole string. */
    if(stream->_flags & __S_NBF) {
        if(write(stream->_fd, s, len) != (ssize_t)len) {
            stream->_flags |= __S_ERR;
            ret = EOF;
        }
        goto cleanup;
    }

    /* Logic for buffered streams */
    const unsigned char *p = (const unsigned char *)s;
    while(*p) {
        if(stream->_cnt == 0) {
            if(__stdio_flush_impl(stream) != 0) {
                ret = EOF;
                goto cleanup;
            }
        }

        /* Optimized write: copy as much as possible into the buffer */
        size_t chunk_len = (size_t)(p - (const unsigned char *)s);
        size_t remaining_in_string = len - chunk_len;
        size_t space_in_buffer = stream->_cnt;

        size_t to_copy = (remaining_in_string < space_in_buffer) ? remaining_in_string : space_in_buffer;

        memcpy(stream->_ptr, p, to_copy);
        stream->_ptr += to_copy;
        stream->_cnt -= to_copy;
        p += to_copy;
        stream->_flags |= __S_DIRTY;

        if((stream->_flags & __S_LBF) && memchr(p - to_copy, '\n', to_copy)) {
            if(__stdio_flush_impl(stream) != 0) {
                ret = EOF;
                goto cleanup;
            }
        }
    }

    /* If buffer became full, flush it */
    if(stream->_cnt == 0) {
        if(__stdio_flush_impl(stream) != 0) {
            ret = EOF;
        }
    }

cleanup:
    _spin_unlock(&stream->_lock);
    return ret;
}
