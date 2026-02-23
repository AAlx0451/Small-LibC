#include <stdio.h>
#include <unistd.h>

off_t ftello(FILE *stream) {
    off_t pos;
    _spin_lock(&stream->_lock);

    // For an accurate position, flush write buffers
    if(stream->_flags & __S_WR) {
        if(__stdio_flush_impl(stream) != 0) {
            pos = (off_t)-1;
            goto cleanup;
        }
    }

    pos = lseek(stream->_fd, 0, SEEK_CUR);
    if(pos == (off_t)-1) {
        goto cleanup;
    }

    // If in read mode, the logical position is the physical position
    // minus the number of bytes remaining in the buffer.
    if(stream->_flags & __S_RD) {
        pos -= stream->_cnt;
    }

cleanup:
    _spin_unlock(&stream->_lock);
    return pos;
}
