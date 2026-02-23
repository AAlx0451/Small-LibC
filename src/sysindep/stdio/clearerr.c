#include <stdio.h>

void clearerr(FILE *p) {
    p->_flags &= ~(__S_ERR | __S_EOF);
}
