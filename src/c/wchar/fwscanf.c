#include <wchar.h>
#include <stdarg.h>
#include <stdio.h>

int fwscanf(FILE *stream, const wchar_t *format, ...) {
    va_list arg;
    int done;

    va_start(arg, format);
    done = vfwscanf(stream, format, arg);
    va_end(arg);

    return done;
}
