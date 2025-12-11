#include <stdio.h>

extern void _spin_lock(volatile int *lock);
extern void _spin_unlock(volatile int *lock);

int fputc(int c, FILE *f) {
    int ret = (unsigned char)c;

    if (!f) return EOF;

    _spin_lock(&f->_lock);
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
