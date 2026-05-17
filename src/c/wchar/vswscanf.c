#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>

int vswscanf(const wchar_t *restrict ws, const wchar_t *restrict format, va_list arg) {
    FILE f;
    memset(&f, 0, sizeof(FILE)); // clears _mode and _mbstate
    f._flags = __S_RD | __S_STR;
    f._fd = -1;
    f._lock = 0;

    f._base = __deconst(unsigned char *, ws);
    f._ptr = __deconst(unsigned char *, ws);

    /* length in wide characters */
    size_t len = 0;
    while(ws[len])
        len++;

    f._cnt = len;
    f._bsize = len;

    return vfwscanf(&f, format, arg);
}
