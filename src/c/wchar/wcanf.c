#include <stdarg.h>
#include <stdio.h>
#include <wchar.h>

int wscanf(const wchar_t *format, ...) {
    va_list arg;
    int done;

    va_start(arg, format);
    done = vwscanf(format, arg);
    va_end(arg);

    return done;
}
