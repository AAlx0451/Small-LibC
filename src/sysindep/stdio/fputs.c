#include <stdio.h>
#include <sys/types.h>

int fputs(const char *s, FILE *stream) {
    if (s == NULL) {
        return EOF;
    }

    _spin_lock(&stream->_lock);
    if (stream->_flags & __S_RD) {
        stream->_flags &= ~__S_RD;
        stream->_flags |= __S_WR;
        stream->_cnt = stream->_bsize;
        stream->_ptr = stream->_base;
    } else if (!(stream->_flags & __S_WR)) {
        stream->_flags |= __S_WR;
        if (stream->_cnt == 0 && stream->_ptr == stream->_base) {
             stream->_cnt = stream->_bsize;
        }
    }
    
    if (*s != '\0') {
        stream->_flags |= __S_DIRTY;
    }

    const unsigned char *p = (const unsigned char *)s;
    while (*p) {
        if (stream->_cnt == 0) {
            if (__stdio_flush_impl(stream) == EOF) {
                _spin_unlock(&stream->_lock);
                return EOF;
            }
            stream->_cnt = stream->_bsize;
        }

        *stream->_ptr++ = *p;
        stream->_cnt--;

        if ((stream->_flags & __S_LBF) && *p == '\n') {
            if (__stdio_flush_impl(stream) == EOF) {
                 _spin_unlock(&stream->_lock);
                 return EOF;
            }
            stream->_cnt = stream->_bsize;
        }
        p++;
    }

    _spin_unlock(&stream->_lock);
    return 0;
}
