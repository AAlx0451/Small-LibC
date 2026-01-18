#include <stdio.h>

int feof(FILE *p) {
    return (p->_flags & __S_EOF);
}
