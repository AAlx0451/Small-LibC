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
                if(chars_read == 0) {
                    p = NULL; // Error or EOF before anything was read.
                }
                goto done;
            }
        }

        unsigned char c = *stream->_ptr++;
        stream->_cnt--;
        *p++ = c;
        chars_read++;

        if(c == '\n') {
            break;
        }
    }

done:
    *p = '\0';
    _spin_unlock(&stream->_lock);
    return (chars_read > 0) ? s : NULL;
}
