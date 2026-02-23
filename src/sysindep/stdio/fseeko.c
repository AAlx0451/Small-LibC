#include <stdio.h>
#include <unistd.h>

int fseeko(FILE *stream, off_t offset, int whence) {
    int result = -1;
    _spin_lock(&stream->_lock);

    // If writing, we must flush the buffer first.
    if(stream->_flags & __S_WR) {
        if(__stdio_flush_impl(stream) != 0) {
            goto cleanup;
        }
    }

    // Clear BOTH direction flags. The stream is now neutral.
    // The next operation (fread/fwrite) will determine the direction.
    stream->_flags &= ~(__S_WR | __S_RD);

    // Invalidate buffer
    stream->_cnt = 0;
    stream->_ptr = stream->_base;

    if(lseek(stream->_fd, offset, whence) != (off_t)-1) {
        stream->_flags &= ~__S_EOF;
        result = 0;
    }

cleanup:
    _spin_unlock(&stream->_lock);
    return result;
}
