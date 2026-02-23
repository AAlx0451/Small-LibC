#include <errno.h>
#include <limits.h>
#include <stdio.h>

long ftell(FILE *stream) {
    off_t pos = ftello(stream);
    if(pos > LONG_MAX) {
        errno = EOVERFLOW;
        return -1L;
    }

    return (long)pos;
}
