#include <stdio.h>
#include <string.h>

size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    size_t total_to_read, bytes_read = 0, chunk;
    unsigned char *cptr = (unsigned char *)ptr;
    if(size == 0 || nmemb == 0) {
        return 0;
    }

    total_to_read = size * nmemb;
    if(nmemb > 0 && total_to_read / nmemb != size) {
        // Overflow check, usually setting errno is better but flag works
        // Note: Locking needed before setting flags if accessed concurrently,
        // but here checking args first is common optimization.
        return 0;
    }

    _spin_lock(&stream->_lock);

    // Check for wrong mode first
    if(stream->_flags & __S_WR) {
        // Cannot read directly if in Write mode without flush/fseek
        stream->_flags |= __S_ERR;
        _spin_unlock(&stream->_lock);
        return 0;
    }

    // If not in Read mode (and not in Write due to check above),
    // switch to Read mode (Neutral -> Read)
    if(!(stream->_flags & __S_RD)) {
        stream->_flags |= __S_RD;
        stream->_cnt = 0;
        stream->_ptr = stream->_base;
    }

    while(bytes_read < total_to_read) {
        if(stream->_cnt <= 0) {
            if(__stdio_fill_impl(stream) == EOF) {
                break;
            }
        }

        chunk = (total_to_read - bytes_read) < stream->_cnt ? (total_to_read - bytes_read) : stream->_cnt;
        memcpy(cptr + bytes_read, stream->_ptr, chunk);

        stream->_ptr += chunk;
        stream->_cnt -= chunk;
        bytes_read += chunk;
    }

    _spin_unlock(&stream->_lock);
    return bytes_read / size;
}
