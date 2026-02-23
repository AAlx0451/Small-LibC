#include <stdarg.h>
#include <stdio.h>

int vsnprintf(char *str, size_t size, const char *format, va_list ap) {
    FILE f;
    int ret;
    f._flags = __S_WR | __S_STR;
    f._fd = -1;
    f._lock = 0;
    f._next = NULL;

    if(size > 0) {
        f._ptr = (unsigned char *)str;
        f._base = (unsigned char *)str;
        f._cnt = size - 1;
        f._bsize = size - 1;
    } else {
        f._ptr = NULL;
        f._base = NULL;
        f._cnt = 0x7FFFFFFF;
        f._bsize = 0;
    }

    ret = vfprintf(&f, format, ap);

    if(size > 0) {
        if(f._ptr < f._base + size) {
            *f._ptr = '\0';
        } else {
            str[size - 1] = '\0';
        }
    }

    __stdio_remove_file(&f);

    return ret;
}
