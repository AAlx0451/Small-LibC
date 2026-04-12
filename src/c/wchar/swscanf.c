#include <stdarg.h>
#include <wchar.h>

int swscanf(const wchar_t *s, const wchar_t *format, ...) {
    va_list arg;
    int done;

    va_start(arg, format);
    done = vswscanf(s, format, arg);
    va_end(arg);

    return done;
}
