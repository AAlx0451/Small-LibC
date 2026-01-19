#include <stdio.h>

int fgetc(FILE *f) {
    int c;

    if (!f) return EOF;

    _spin_lock(&f->_lock);

    if (f->_cnt > 0) {
        c = *f->_ptr++;
        f->_cnt--;
    } else {
        if (__stdio_fill_impl(f) == 0) {
            c = *f->_ptr++;
            f->_cnt--;
        } else {
            c = EOF;
        }
    }

    _spin_unlock(&f->_lock);
    return c;
}
