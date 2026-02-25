#include <stdarg.h>
#include <stdio.h>

int asprintf(char **strp, const char *fmt, ...) {
    int result;
    va_list ap;
    va_start(ap, fmt);
    result = vasprintf(strp, fmt, ap);
    va_end(ap);
    return result;
}
