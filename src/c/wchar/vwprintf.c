#include <stdarg.h>
#include <stdio.h>
#include <wchar.h>

int vwprintf(const wchar_t *format, va_list arg) {
    return vfwprintf(stdout, format, arg);
}
