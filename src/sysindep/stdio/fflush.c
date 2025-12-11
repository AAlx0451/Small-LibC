#include <stdio.h>

int fflush(FILE *stream) {
    if (!stream) {
        __stdio_flush_all();
        return 0;
    }

    if (!(stream->_flags & __S_WR)) {
        return 0;
    }

    return __stdio_flush_impl(stream);
}
