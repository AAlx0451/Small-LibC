#include <stddef.h>
#include <stdio.h>
#include <wchar.h>

char *gets(char *s);

char *gets(char *s) {
    char *p;
    int chars_read = 0, c;
    if(s == NULL) {
        return NULL;
    }

    p = s;

    int mode = fwide(stdin, 0);
    if (mode > 0) return NULL;
    else if (mode == 0) fwide(stdin, -1);

    _spin_lock(&stdin->_lock);

    for(;;) {
        if(stdin->_cnt <= 0) {
            if(__stdio_fill_impl(stdin) == EOF) {
                c = EOF;
                break;
            }
        }

        c = *stdin->_ptr++;
        stdin->_cnt--;

        if(c == '\n' || c == EOF) {
            break;
        }

        *p++ = (char)c;
        chars_read++;
    }

    *p = '\0';
    _spin_unlock(&stdin->_lock);

    if(c == EOF && chars_read == 0) {
        return NULL;
    }

    return s;
}
