#include <wchar.h>
#include <stdarg.h>

int swprintf(wchar_t *s, size_t n, const wchar_t *format, ...)
{
    va_list arg;
    int done;

    va_start(arg, format);
    done = vswprintf(s, n, format, arg);
    va_end(arg);

    return done;
}
