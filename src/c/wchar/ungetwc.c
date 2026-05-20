#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <wchar.h>

wint_t ungetwc(wint_t c, FILE *f)
{
    if (c == WEOF || !f)
        return WEOF;

    int mode = fwide(f, 0);
    if (mode < 0)
        return WEOF;
    else if (mode == 0)
        fwide(f, 1);

    _spin_lock(&f->_lock);

    /* Convert wide char to multibyte sequence.
     * We use a temporary mbstate so we don't mess up the stream's forward parsing state! */
    char buf[MB_LEN_MAX];
    mbstate_t temp_mbs = {0};
    size_t len = wcrtomb(buf, (wchar_t)c, &temp_mbs);

    if (len == (size_t)-1) {
        _spin_unlock(&f->_lock);
        return WEOF;
    }

    /* Use the MB_LEN_MAX reserve hint left in the original ungetc code */
    int reserve_size = (f->_flags & __S_RESERVE) ? MB_LEN_MAX : 0;
    unsigned char *min_ptr = f->_base - reserve_size;

    /* Check if we have enough space to push back 'len' bytes */
    if (f->_base && (f->_ptr - len) >= min_ptr) {
        f->_ptr -= len;

        /* Copy multibyte sequence back into the buffer */
        for (size_t i = 0; i < len; i++) {
            f->_ptr[i] = (unsigned char)buf[i];
        }

        f->_cnt += len;
        f->_flags &= ~__S_EOF;

        _spin_unlock(&f->_lock);
        return c;
    }

    _spin_unlock(&f->_lock);
    return WEOF;
}
