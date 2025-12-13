#include <stdio.h>
#include <stdarg.h>

int printf(const char *format, ...) {
    va_list ap;
    int ret;
    va_start(ap, format);
    ret = vfprintf(stdout, format, ap);
    va_end(ap);
    return ret;
}

int fprintf(FILE *stream, const char *format, ...) {
    va_list ap;
    int ret;
    va_start(ap, format);
    ret = vfprintf(stream, format, ap);
    va_end(ap);
    return ret;
}

int vprintf(const char *format, va_list ap) {
    return vfprintf(stdout, format, ap);
}

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
    f._lock = 0; // Explicitly initialize lock on stack-allocated FILE

    va_start(ap, format);
    ret = vfprintf(&f, format, ap);
    va_end(ap);

    *f._ptr = '\0';
    return ret;
}

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

    ret = vfprintf(&f, format, ap);
    *f._ptr = '\0';
    return ret;
}

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

    va_start(ap, format);
    ret = vfprintf(&f, format, ap);
    va_end(ap);

    if (f._ptr < f._base + size) {
        *f._ptr = '\0';
    } else {
        str[size - 1] = '\0';
    }
    return ret;
}

int vsnprintf(char *str, size_t size, const char *format, va_list ap) {
    FILE f;
    int ret;

    if (size == 0) return 0;

    f._flags = __S_WR | __S_STR;
    f._ptr = (unsigned char *)str;
    f._base = (unsigned char *)str;
    f._cnt = size - 1;
    f._bsize = size - 1;
    f._fd = -1;
    f._lock = 0;

    ret = vfprintf(&f, format, ap);

    if (f._ptr < f._base + size) {
        *f._ptr = '\0';
    } else {
        str[size - 1] = '\0';
    }
    return ret;
}
