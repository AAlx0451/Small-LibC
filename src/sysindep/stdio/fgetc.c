#include <stdio.h>
#include <errno.h>

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

int getc(FILE *f) {
    return fgetc(f);
}

int getchar(void) {
    return fgetc(stdin);
}

int ungetc(int c, FILE *f) {
    if (c == EOF || !f) return EOF;

    _spin_lock(&f->_lock);

    // Allow ungetc even if ptr == base, provided we have a reserve byte (set in fopen)
    // If we have reserve, valid range is >= base - 1.
    int has_reserve = (f->_flags & __S_RESERVE) ? 1 : 0;
    unsigned char *min_ptr = f->_base - has_reserve;

    if (f->_base && f->_ptr > min_ptr) {
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
