#include <stdio.h>

int fseek(FILE *stream, long offset, int whence) {
    return fseeko(stream, (off_t)offset, whence);
}
