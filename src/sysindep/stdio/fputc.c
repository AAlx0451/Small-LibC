#include <stdio.h>
#include <unistd.h> /* for write() */

int fputc(int c, FILE *f) {
    unsigned char ch = (unsigned char)c;
    int ret = (int)ch;

    if (!f) return EOF;

    _spin_lock(&f->_lock);

    /* Ensure stream is in write mode (same logic as before) */
    if (f->_flags & __S_RD) {
        f->_flags &= ~__S_RD;
        f->_flags |= __S_WR;
        f->_cnt = f->_bsize;
        f->_ptr = f->_base;
    } else if (!(f->_flags & __S_WR)) {
        f->_flags |= __S_WR;
        if (f->_cnt == 0 && f->_ptr == f->_base) {
             f->_cnt = f->_bsize;
        }
    }

    /* If stream is unbuffered, write directly and we are done. */
    if (f->_flags & __S_NBF) {
        if (write(f->_fd, &ch, 1) != 1) {
            ret = EOF;
            f->_flags |= __S_ERR;
        }
        goto cleanup; /* Skip all buffer logic */
    }

    /* Logic for buffered streams (_IOFBF, _IOLBF) */

    /* If buffer is full, flush it. */
    if (f->_cnt == 0) {
        if (__stdio_flush_impl(f) != 0) {
            ret = EOF;
            goto cleanup;
        }
    }

    /* Write character to buffer */
    *f->_ptr++ = ch;
    f->_cnt--;
    f->_flags |= __S_DIRTY;

    /* If buffer becomes full now, or it's line-buffered and we got a newline, flush */
    if (f->_cnt == 0 || ((f->_flags & __S_LBF) && c == '\n')) {
        if (__stdio_flush_impl(f) != 0) {
            ret = EOF;
        }
    }

cleanup:
    _spin_unlock(&f->_lock);
    return ret;
}
