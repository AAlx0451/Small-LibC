#include <stdio.h>

void rewind(FILE *stream) {
    (void)fseeko(stream, 0L, SEEK_SET);
    _spin_lock(&stream->_lock);
    stream->_flags &= ~__S_ERR;
    _spin_unlock(&stream->_lock);
}
