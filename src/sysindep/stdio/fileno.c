#include <stdio.h>

int fileno(FILE *p) {
    return (p->_fd);
}
