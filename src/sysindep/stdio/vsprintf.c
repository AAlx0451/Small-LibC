#include <stdio.h>
#include <stdarg.h>

int vsprintf(char *str, const char *format, va_list ap) {
    FILE f;
    int ret;

    f._flags = __S_WR | __S_STR;
    f._ptr = (unsigned char *)str;
    f._base = (unsigned char *)str;
    f._cnt = 0x7FFFFFFF;
    f._bsize = 0x7FFFFFFF;
    f._fd = -1;
    f._lock = 0;
    f._next = NULL;

    ret = vfprintf(&f, format, ap);
    *f._ptr = '\0';

    __stdio_remove_file(&f);

    return ret;
}
