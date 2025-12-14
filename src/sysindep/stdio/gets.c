#include <stdio.h>
#include <stddef.h>

char *gets(char *s) {
    if (s == NULL) {
        return NULL;
    }
    
    char *p = s;
    int c;
    int chars_read = 0;

    _spin_lock(&stdin->_lock);

    for (;;) {
        if (stdin->_cnt <= 0) {
            if (__stdio_fill_impl(stdin) == EOF) {
                c = EOF;
                break;
            }
        }
        
        c = *stdin->_ptr++;
        stdin->_cnt--;

        if (c == '\n' || c == EOF) {
            break;
        }
        
        *p++ = c;
        chars_read++;
    }

    *p = '\0';
    _spin_unlock(&stdin->_lock);

    if (c == EOF && chars_read == 0) {
        return NULL;
    }
    
    return s;
}
