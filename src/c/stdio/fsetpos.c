#include <stdio.h>
#include <sys/types.h>

int fsetpos(FILE *stream, const fpos_t *pos) {
    if(stream == NULL || pos == NULL) {
        return -1;
    }

    if(fseeko(stream, pos->_pos, SEEK_SET) == -1) {
        return -1;
    }

    stream->_mbstate = pos->_state;

    return 0;
}
