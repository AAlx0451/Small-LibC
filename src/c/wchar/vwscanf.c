#include <stdio.h>
#include <wchar.h>
#include <stdarg.h>

int vwscanf(const wchar_t *format, va_list arg) {
    return vfwscanf(stdin, format, arg);
}
