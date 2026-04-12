#include <stdio.h>
#include <sys/types.h>

int fgetpos(FILE *stream, fpos_t *pos) {
    off_t offset;

    if(stream == NULL || pos == NULL) {
        return -1;
    }

    offset = ftello(stream);
    if(offset == -1) {
        return -1;
    }

    pos->_pos = offset;
    pos->_state = stream->_mbstate;

    return 0;
}
