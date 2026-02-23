#include <stdarg.h>
#include <stdio.h>

int fscanf(FILE *stream, const char *format, ...) {
    int ret;
    va_list ap;
    va_start(ap, format);
    ret = vfscanf(stream, format, ap);
    va_end(ap);
    return ret;
}
