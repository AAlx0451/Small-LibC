#include <stdio.h>

void clearerr(FILE *f) {
    _spin_lock(&f->_lock);

    f->_flags &= ~(__S_ERR | __S_EOF);

    _spin_unlock(&f->_lock);
}
