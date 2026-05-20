#include <stdarg.h>
#include <stdio.h>
#include <wchar.h>

int fwprintf(FILE *stream, const wchar_t *format, ...)
{
    va_list args;
    int result;

    va_start(args, format);

    result = vfwprintf(stream, format, args);

    va_end(args);

    return result;
}
