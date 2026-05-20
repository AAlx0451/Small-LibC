#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <unistd.h> /* for write() */
#include <wchar.h>

int fputws(const wchar_t *restrict ws, FILE *restrict stream)
{
    int ret = 0;
    char buf[MB_LEN_MAX];

    if (ws == NULL || stream == NULL) {
        return EOF;
    }

    /* Fast check for empty string */
    if (*ws == L'\0') {
        return 0;
    }

    /* Wide orientation check */
    int mode = fwide(stream, 0);
    if (mode < 0)
        return EOF;
    else if (mode == 0)
        fwide(stream, 1);

    _spin_lock(&stream->_lock);

    /* Ensure stream is in write mode */
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

    /* Unbuffered stream (__S_NBF) optimization:
     * Instead of a syscall per wide char, we use a small local stack buffer
     * to accumulate the multibyte string, then flush it out via write().
     * This avoids allocations and mirrors the efficiency of fputs. */
    if (stream->_flags & __S_NBF) {
        char chunk[128];
        size_t c_idx = 0;

        while (*ws) {
            size_t len = wcrtomb(buf, *ws, &stream->_mbstate);
            if (len == (size_t)-1) {
                stream->_flags |= __S_ERR;
                errno = EILSEQ;
                ret = EOF;
                goto cleanup;
            }

            /* If chunk is full, flush to file descriptor */
            if (c_idx + len > sizeof(chunk)) {
                if (write(stream->_fd, chunk, c_idx) != (ssize_t)c_idx) {
                    stream->_flags |= __S_ERR;
                    ret = EOF;
                    goto cleanup;
                }
                c_idx = 0;
            }

            /* Append multibyte sequence to chunk */
            for (size_t i = 0; i < len; i++) {
                chunk[c_idx++] = buf[i];
            }
            ws++;
        }

        /* Flush any remaining bytes in chunk */
        if (c_idx > 0) {
            if (write(stream->_fd, chunk, c_idx) != (ssize_t)c_idx) {
                stream->_flags |= __S_ERR;
                ret = EOF;
            }
        }
        goto cleanup; /* Skip buffered logic */
    }

    /* Logic for buffered streams */
    while (*ws) {
        size_t len = wcrtomb(buf, *ws, &stream->_mbstate);
        if (len == (size_t)-1) {
            stream->_flags |= __S_ERR;
            errno = EILSEQ;
            ret = EOF;
            goto cleanup;
        }

        /* Push converted bytes into the stream buffer */
        for (size_t i = 0; i < len; i++) {
            if (stream->_cnt == 0) {
                if (__stdio_flush_impl(stream) != 0) {
                    ret = EOF;
                    goto cleanup;
                }
            }

            *stream->_ptr++ = (unsigned char)buf[i];
            stream->_cnt--;
            stream->_flags |= __S_DIRTY;

            /* Flush if line-buffered and we hit a newline */
            if ((stream->_flags & __S_LBF) && buf[i] == '\n') {
                if (__stdio_flush_impl(stream) != 0) {
                    ret = EOF;
                    goto cleanup;
                }
            }
        }
        ws++;
    }

    /* Final check: if buffer became full exactly at the end */
    if (stream->_cnt == 0) {
        if (__stdio_flush_impl(stream) != 0) {
            ret = EOF;
        }
    }

cleanup:
    _spin_unlock(&stream->_lock);
    return ret;
}
