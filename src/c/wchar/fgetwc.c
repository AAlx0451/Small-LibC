#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <wchar.h>

wint_t fgetwc(FILE *f)
{
    if (!f)
        return WEOF;

    /* Check and set wide orientation */
    int mode = fwide(f, 0);
    if (mode < 0)
        return WEOF;
    else if (mode == 0)
        fwide(f, 1);

    _spin_lock(&f->_lock);

    wchar_t wc;
    size_t res;
    int byte_read;
    char c_byte;
    size_t bytes_consumed = 0;

    /* We read byte-by-byte into mbrtowc to properly handle stream buffering */
    while (1) {
        /* Inline fgetc buffer logic to avoid dropping the lock */
        if (f->_cnt > 0) {
            byte_read = *f->_ptr++;
            f->_cnt--;
        } else {
            if (__stdio_fill_impl(f) == 0) {
                byte_read = *f->_ptr++;
                f->_cnt--;
            } else {
                byte_read = EOF;
            }
        }

        if (byte_read == EOF) {
            if (bytes_consumed == 0) {
                /* Normal EOF at the start of character */
                _spin_unlock(&f->_lock);
                return WEOF;
            } else {
                /* Unexpected EOF in the middle of a multibyte sequence */
                f->_flags |= __S_ERR;
                errno = EILSEQ;
                _spin_unlock(&f->_lock);
                return WEOF;
            }
        }

        c_byte = (char)byte_read;
        bytes_consumed++;

        /* Feed one byte to mbrtowc. It maintains state in f->_mbstate */
        res = mbrtowc(&wc, &c_byte, 1, &f->_mbstate);

        if (res == (size_t)-1) {
            /* Illegal byte sequence */
            f->_flags |= __S_ERR;
            errno = EILSEQ;
            _spin_unlock(&f->_lock);
            return WEOF;
        }

        if (res == (size_t)-2) {
            /* Incomplete multibyte character, need more bytes, continue loop */
            continue;
        }

        /* Success: res is 0 (for L'\0') or 1 (since we feed 1 byte at a time) */
        break;
    }

    _spin_unlock(&f->_lock);
    return (wint_t)wc;
}
