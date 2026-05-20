#include <stdio.h>
#include <wchar.h>

int fwide(FILE *f, int mode)
{
    _spin_lock(&f->_lock);

    if (mode) {
        if (!f->_mode)
            f->_mode = mode > 0 ? 1 : -1;
    }

    mode = f->_mode;

    _spin_unlock(&f->_lock);

    return mode;
}
