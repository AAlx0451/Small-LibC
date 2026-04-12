#include <stdio.h>
#include <wchar.h>
#include <stdarg.h>

int vwprintf(const wchar_t *format, va_list arg) {
    return vfwprintf(stdout, format, arg);
}
