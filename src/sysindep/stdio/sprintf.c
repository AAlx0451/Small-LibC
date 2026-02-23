#include <stdarg.h>
#include <stdio.h>

int sprintf(char *str, const char *format, ...) {
    FILE f;
    va_list ap;
    int ret;

    f._flags = __S_WR | __S_STR;
    f._ptr = (unsigned char *)str;
    f._base = (unsigned char *)str;
    f._cnt = 0x7FFFFFFF;
    f._bsize = 0x7FFFFFFF;
    f._fd = -1;
    f._lock = 0;
    f._next = NULL;

    va_start(ap, format);
    ret = vfprintf(&f, format, ap);
    va_end(ap);

    *f._ptr = '\0';
    __stdio_remove_file(&f);

    return ret;
}
