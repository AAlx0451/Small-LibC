#include <stddef.h>
#include <stdio.h>
#include <wchar.h>

char *fgets(char *s, int size, FILE *stream) {
    char *p;
    int chars_read = 0;
    unsigned char c;
    if(s == NULL || size <= 0) {
        return NULL;
    }

    int mode = fwide(stream, 0);
    if(mode > 0)
        return NULL;
    else if(mode == 0)
        fwide(stream, -1);

    p = s;
    _spin_lock(&stream->_lock);

    // Leave space for the null terminator.
    while(--size > 0) {
        if(stream->_cnt <= 0) {
            if(__stdio_fill_impl(stream) == EOF) {
                goto done;
            }
        }

        c = *stream->_ptr++;
        stream->_cnt--;
        *p++ = (char)c;
        chars_read++;

        if(c == '\n') {
            break;
        }
    }

done:
    if(p)
        *p = '\0';
    _spin_unlock(&stream->_lock);
    return (chars_read > 0) ? s : NULL;
}
