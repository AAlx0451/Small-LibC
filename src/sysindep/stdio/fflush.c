#include <stdio.h>

extern void _spin_lock(volatile int *lock);
extern void _spin_unlock(volatile int *lock);

int fflush(FILE *stream) {
    if (!stream) {
        __stdio_flush_all();
        return 0;
    }

    // Acquire lock BEFORE checking flags to prevent race with freopen/mode change
    _spin_lock(&stream->_lock);

    if (!(stream->_flags & __S_WR)) {
        _spin_unlock(&stream->_lock);
        return 0;
    }

    int ret = __stdio_flush_impl(stream);
    _spin_unlock(&stream->_lock);
    return ret;
}
