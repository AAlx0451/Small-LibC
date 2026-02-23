#include <stdio.h>
#include <string.h>
#include <unistd.h>

size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream) {
    size_t to_write, space;
    const unsigned char *p = (const unsigned char *)ptr;
    size_t total_bytes;
    size_t bytes_written = 0;

    if(!stream || !ptr || size == 0 || nmemb == 0)
        return 0;

    _spin_lock(&stream->_lock);

    total_bytes = size * nmemb;
    to_write = total_bytes;

    // Handle both Read->Write transition AND Neutral->Write transition
    if(stream->_flags & __S_RD) {
        stream->_flags &= ~__S_RD;
        stream->_flags |= __S_WR;
        stream->_cnt = stream->_bsize;
        stream->_ptr = stream->_base;
    } else if(!(stream->_flags & __S_WR)) {
        // Stream was neutral, set to Write
        stream->_flags |= __S_WR;
        // If pointers weren't initialized (e.g. after fseek cleanup), init them
        if(stream->_cnt == 0 && stream->_ptr == stream->_base) {
            stream->_cnt = stream->_bsize;
        }
    }

    while(to_write > 0) {
        space = stream->_cnt;
        if(to_write <= space) {
            memcpy(stream->_ptr, p, to_write);
            stream->_ptr += to_write;
            stream->_cnt -= to_write;
            stream->_flags |= __S_DIRTY;
            bytes_written += to_write;
            break;
        }

        if(space > 0) {
            memcpy(stream->_ptr, p, space);
            stream->_ptr += space;
            stream->_cnt = 0;
            stream->_flags |= __S_DIRTY;
            p += space;
            to_write -= space;
            bytes_written += space;
        }

        if(__stdio_flush_impl(stream) == EOF) {
            break;
        }

        if(to_write >= stream->_bsize) {
            ssize_t w = write(stream->_fd, p, to_write);
            if(w <= 0) {
                stream->_flags |= __S_ERR;
                break;
            }
            p += w;
            to_write -= w;
            bytes_written += w;
        }
    }

    _spin_unlock(&stream->_lock);
    return bytes_written / size;
}
