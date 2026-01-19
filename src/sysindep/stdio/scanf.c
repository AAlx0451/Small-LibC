#include <stdio.h>
#include <stdarg.h>

int scanf(const char *format, ...) {
    int ret;
    va_list ap;
    va_start(ap, format);
    ret = vfscanf(stdin, format, ap);
    va_end(ap);
    return ret;
}
