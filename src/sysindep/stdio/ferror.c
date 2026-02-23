#include <stdio.h>

int ferror(FILE *p) {
    return (p->_flags & __S_ERR);
}
