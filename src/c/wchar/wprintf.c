#include <stdarg.h>
#include <stdio.h>
#include <wchar.h>

int wprintf(const wchar_t *format, ...)
{
    va_list args;
    int done;

    va_start(args, format);
    done = vfwprintf(stdout, format, args);
    va_end(args);

    return done;
}
