#include <stdio.h>

int fputc(int c, FILE *f) {
    int ret = (unsigned char)c;

    if (!f) return EOF;

    _spin_lock(&f->_lock);
    /* This handles both R->W and Neutral->W transitions. */
    if (f->_flags & __S_RD) {
        f->_flags &= ~__S_RD;
        f->_flags |= __S_WR;
        f->_cnt = f->_bsize;
        f->_ptr = f->_base;
    } else if (!(f->_flags & __S_WR)) {
        f->_flags |= __S_WR;
        // Initialize buffer pointers if they were cleared by a seek operation
        if (f->_cnt == 0 && f->_ptr == f->_base) {
             f->_cnt = f->_bsize;
        }
    }

    if (f->_cnt <= 1 && (f->_flags & __S_NBF)) {
        if (__stdio_flush_impl(f) != 0) {
             ret = EOF;
             goto cleanup;
        }
        *f->_ptr++ = (unsigned char)c;
        f->_flags |= __S_DIRTY;
        if (__stdio_flush_impl(f) != 0) {
            ret = EOF;
        }
        goto cleanup;
    }

    if (f->_cnt == 0) {
        if (f->_flags & __S_STR) {
            ret = EOF;
            goto cleanup;
        }
        if (__stdio_flush_impl(f) != 0) {
            ret = EOF;
            goto cleanup;
        }
    }

    *f->_ptr++ = (unsigned char)c;
    f->_cnt--;
    f->_flags |= __S_DIRTY;

    if ((f->_flags & __S_LBF && c == '\n') || f->_cnt == 0) {
        if (__stdio_flush_impl(f) != 0) {
            ret = EOF;
        }
    }

cleanup:
    _spin_unlock(&f->_lock);
    return ret;
}
