#include <errno.h>
#include <stdio.h>
#include <wchar.h>

wchar_t *fgetws(wchar_t *restrict ws, int n, FILE *restrict stream)
{
    wchar_t *p;
    int chars_read = 0;

    if (ws == NULL || n <= 0 || stream == NULL) {
        return NULL;
    }

    /* Wide orientation check */
    int mode = fwide(stream, 0);
    if (mode < 0)
        return NULL;
    else if (mode == 0)
        fwide(stream, 1);

    p = ws;
    _spin_lock(&stream->_lock);

    /* Leave space for the null terminator. n represents wide chars, not bytes. */
    while (--n > 0) {
        wchar_t wc;
        size_t res;
        int byte_read;
        char c_byte;
        size_t bytes_consumed = 0;

        /* State machine: fetch byte-by-byte into mbrtowc */
        while (1) {
            if (stream->_cnt > 0) {
                byte_read = *stream->_ptr++;
                stream->_cnt--;
            } else {
                if (__stdio_fill_impl(stream) == 0) {
                    byte_read = *stream->_ptr++;
                    stream->_cnt--;
                } else {
                    byte_read = EOF;
                }
            }

            if (byte_read == EOF) {
                if (bytes_consumed == 0) {
                    /* EOF at the very start of reading a wide character is normal */
                    goto done;
                } else {
                    /* EOF in the middle of a multibyte sequence -> encoding error */
                    stream->_flags |= __S_ERR;
                    errno = EILSEQ;
                    goto error_out;
                }
            }

            c_byte = (char)byte_read;
            bytes_consumed++;

            res = mbrtowc(&wc, &c_byte, 1, &stream->_mbstate);

            if (res == (size_t)-1) {
                /* Invalid multibyte sequence */
                stream->_flags |= __S_ERR;
                errno = EILSEQ;
                goto error_out;
            }

            if (res == (size_t)-2) {
                /* Incomplete sequence, loop to read next byte */
                continue;
            }

            /* Character successfully parsed */
            break;
        }

        *p++ = wc;
        chars_read++;

        /* fgetws includes the newline character, then stops */
        if (wc == L'\n') {
            break;
        }
    }

done:
    if (p)
        *p = L'\0';
    _spin_unlock(&stream->_lock);
    /* Return string if we read anything, else pure EOF returns NULL */
    return (chars_read > 0) ? ws : NULL;

error_out:
    /* For standard POSIX, fgetws on encoding error returns NULL immediately */
    _spin_unlock(&stream->_lock);
    return NULL;
}
