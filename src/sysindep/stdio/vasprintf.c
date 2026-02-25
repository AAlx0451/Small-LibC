#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

int vasprintf(char **strp, const char *fmt, va_list ap) {
    int len;
    char *buffer;
    va_list ap_copy;
    va_copy(ap_copy, ap);
    len = vsnprintf(NULL, 0, fmt, ap_copy);
    va_end(ap_copy);

    if(len < 0)
        return -1;

    buffer = (char *)malloc((unsigned)len + 1);
    if(!buffer) {
        return -1;
    }

    vsnprintf(buffer, (unsigned)len + 1, fmt, ap);

    *strp = buffer;
    return len;
}
