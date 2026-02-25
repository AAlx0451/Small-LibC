#include <stdio.h>

int ungetc(int c, FILE *f) {
    int has_reserve;
    unsigned char *min_ptr;
    if(c == EOF || !f)
        return EOF;

    _spin_lock(&f->_lock);

    // Allow ungetc even if ptr == base, provided we have a reserve byte (set in fopen)
    // If we have reserve, valid range is >= base - 1.
    has_reserve = (f->_flags & __S_RESERVE) ? 1 : 0;
    min_ptr = f->_base - has_reserve;
    if(f->_base && f->_ptr > min_ptr) {
        f->_ptr--;
        *f->_ptr = (unsigned char)c;
        f->_cnt++;
        f->_flags &= ~__S_EOF;
        _spin_unlock(&f->_lock);
        return c;
    }

    _spin_unlock(&f->_lock);
    return EOF;
}
