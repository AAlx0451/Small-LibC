#include <stdio.h>
#include <stdarg.h>

int asprintf(char **strp, const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    int result = vasprintf(strp, fmt, ap);
    va_end(ap);
    return result;
}
