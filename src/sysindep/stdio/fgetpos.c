#include <stdio.h>
#include <sys/types.h>

int fgetpos(FILE *stream, fpos_t *pos) {
    off_t offset;

    if(pos == NULL) {
        return -1;
    }

    offset = ftello(stream);

    if(offset == -1) {
        return -1;
    }

    *pos = offset;
    return 0;
}
