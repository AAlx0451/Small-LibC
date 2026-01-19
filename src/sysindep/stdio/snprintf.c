#include <stdio.h>
#include <stdarg.h>

int snprintf(char *str, size_t size, const char *format, ...) {
    FILE f;
    va_list ap;
    int ret;

    if (size == 0) return 0;

    f._flags = __S_WR | __S_STR;
    f._ptr = (unsigned char *)str;
    f._base = (unsigned char *)str;
    f._cnt = size - 1;
    f._bsize = size - 1;
    f._fd = -1;
    f._lock = 0;
    f._next = NULL;

    va_start(ap, format);
    ret = vfprintf(&f, format, ap);
    va_end(ap);

    if (f._ptr < f._base + size) {
        *f._ptr = '\0';
    } else {
        str[size - 1] = '\0';
    }

    __stdio_remove_file(&f);

    return ret;
}
