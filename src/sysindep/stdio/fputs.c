#include <stdio.h>
#include <sys/types.h> // off_t

int fputs(const char *s, FILE *stream) {
    if (s == NULL) {
        return EOF;
    }
    
    _spin_lock(&stream->_lock);
    // a file positioning function must be called to switch
    // from reading to writing on an update stream.
    if (stream->_flags & __S_RD) {
        _spin_unlock(&stream->_lock); // Release lock to call public API.
        if (fseeko(stream, 0, SEEK_CUR) != 0) {
            return EOF;
        }
        _spin_lock(&stream->_lock); // Re-acquire lock to continue operation.
    }

    if (!(stream->_flags & __S_WR)) {
        stream->_flags |= __S_WR;
        stream->_cnt = stream->_bsize;
        stream->_ptr = stream->_base;
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
