#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void _spin_lock(volatile int *lock);
void _spin_unlock(volatile int *lock);

// Use internal flush to avoid recursive locking
extern int __stdio_flush_impl(FILE *stream);
extern void __stdio_free_buffer(FILE *f);

int setvbuf(FILE *stream, char *buf, int mode, size_t size)
{
    _spin_lock(&stream->_lock);

    // FIX: Calling fflush() here caused a deadlock because fflush() 
    // tries to acquire the same lock we just took.
    // Use the internal implementation which assumes lock is held.
    if (stream->_flags & __S_WR) {
        __stdio_flush_impl(stream);
    }

    __stdio_free_buffer(stream);

    stream->_flags &= ~(__S_NBF | __S_LBF);

    switch (mode) {
    case _IONBF:
        stream->_flags |= __S_NBF;
        stream->_base = NULL;
        stream->_bsize = 0;
        break;

    case _IOLBF:
        stream->_flags |= __S_LBF;
        /* Fallthrough */
    case _IOFBF:
        if (size == 0) {
            _spin_unlock(&stream->_lock);
            return -1;
        }
        if (buf == NULL) {
            buf = malloc(size);
            if (buf == NULL) {
                _spin_unlock(&stream->_lock);
                return -1;
            }
            stream->_flags |= __S_FREEBUF;
        }
        stream->_base = (unsigned char*)buf;
        stream->_bsize = size;
        break;

    default:
        _spin_unlock(&stream->_lock);
        return -1;
    }

    stream->_ptr = stream->_base;
    stream->_cnt = (stream->_flags & __S_RD) ? 0 : stream->_bsize;

    _spin_unlock(&stream->_lock);
    return 0;
}
