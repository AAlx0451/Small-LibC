#include <stdio.h>

extern void _spin_lock(volatile int *lock);
extern void _spin_unlock(volatile int *lock);

int fputc(int c, FILE *f) {
    int ret = c;

    if (!f) return EOF;

    _spin_lock(&f->_lock);

    if (f->_cnt > 0) {
        *f->_ptr++ = (unsigned char)c;
        f->_cnt--;
        f->_flags |= __S_DIRTY; /* <--- ВАЖНОЕ ИСПРАВЛЕНИЕ */
    } else {
        if (f->_flags & __S_STR) {
            ret = EOF; 
        } else {
            if (__stdio_flush_impl(f) == 0) {
                *f->_ptr++ = (unsigned char)c;
                f->_cnt--;
                f->_flags |= __S_DIRTY; /* <--- И ЗДЕСЬ */
            } else {
                ret = EOF;
                f->_flags |= __S_ERR;
            }
        }
    }

    _spin_unlock(&f->_lock);
    return ret;
}
