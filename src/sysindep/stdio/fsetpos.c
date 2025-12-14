#include <stdio.h>
#include <sys/types.h>

int fsetpos(FILE *stream, const fpos_t *pos) {
    if (pos == NULL) {
        return -1;
    }
    
    return fseeko(stream, *pos, SEEK_SET);
}
