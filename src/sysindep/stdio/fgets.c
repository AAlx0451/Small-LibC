#include <stddef.h>
#include <stdio.h>

char *fgets(char *s, int size, FILE *stream) {
    if(s == NULL || size <= 0) {
        return NULL;
    }

    char *p = s;
    int chars_read = 0;

    _spin_lock(&stream->_lock);

    // Leave space for the null terminator.
    while(--size > 0) {
        if(stream->_cnt <= 0) {
            if(__stdio_fill_impl(stream) == EOF) {
                goto done;
            }
        }

        unsigned char c = *stream->_ptr++;
        stream->_cnt--;
        *p++ = (char)c;
        chars_read++;

        if(c == '\n') {
            break;
        }
    }

done:
    if(p) *p = '\0';
    _spin_unlock(&stream->_lock);
    return (chars_read > 0) ? s : NULL;
}
