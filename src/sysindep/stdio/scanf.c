#include <stdio.h>
#include <stdarg.h>
#include <string.h>

int scanf(const char *format, ...) {
    int ret;
    va_list ap;
    va_start(ap, format);
    ret = vfscanf(stdin, format, ap);
    va_end(ap);
    return ret;
}

int fscanf(FILE *stream, const char *format, ...) {
    int ret;
    va_list ap;
    va_start(ap, format);
    ret = vfscanf(stream, format, ap);
    va_end(ap);
    return ret;
}

int vscanf(const char *format, va_list arg) {
    return vfscanf(stdin, format, arg);
}

int vsscanf(const char *str, const char *format, va_list arg) {
    FILE f;

    /* Initialize a temporary FILE object for reading from memory */
    f._fd = -1;                 /* No file descriptor */
    f._flags = __S_STR | __S_RD; /* String stream, Read-only */
    f._base = (unsigned char *)str;
    f._ptr  = (unsigned char *)str;
    f._cnt = strlen(str);
    f._bsize = f._cnt;
    f._lock = 0;                /* No locking needed for stack-local struct */

    return vfscanf(&f, format, arg);
}

int sscanf(const char *str, const char *format, ...) {
    int ret;
    va_list ap;
    va_start(ap, format);
    ret = vsscanf(str, format, ap);
    va_end(ap);
    return ret;
}
