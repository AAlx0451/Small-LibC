#include <stdio.h>
#include <stdarg.h>

int sscanf(const char *str, const char *format, ...) {
    int ret;
    va_list ap;
    va_start(ap, format);
    ret = vsscanf(str, format, ap);
    va_end(ap);
    return ret;
}
