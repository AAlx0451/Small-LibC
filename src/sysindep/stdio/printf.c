#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>

int dprintf(int fd, const char *format, ...) {
    va_list args;
    char *buffer = NULL;
    int len;
    ssize_t bytes_written;

    va_start(args, format);
    len = vasprintf(&buffer, format, args);
    va_end(args);

    if (len == -1) {
        return -1;
    }

    bytes_written = write(fd, buffer, len);
    free(buffer);
    
    return bytes_written;
}

int vasprintf(char **strp, const char *fmt, va_list ap) {
    va_list ap_copy;
    va_copy(ap_copy, ap);
    int len = vsnprintf(NULL, 0, fmt, ap_copy);
    va_end(ap_copy);

    if (len < 0) {
        return -1;
    }

    char *buffer = malloc(len + 1);
    if (!buffer) {
        return -1;
    }
    
    vsnprintf(buffer, len + 1, fmt, ap);
    
    *strp = buffer;
    return len;
}

int asprintf(char **strp, const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    int result = vasprintf(strp, fmt, ap);
    va_end(ap);
    return result;
}

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
    f._next = NULL;

    va_start(ap, format);
    ret = vfprintf(&f, format, ap);
    va_end(ap);

    *f._ptr = '\0';
    __stdio_remove_file(&f);
    
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
    f._next = NULL;

    ret = vfprintf(&f, format, ap);
    *f._ptr = '\0';
    
    __stdio_remove_file(&f);

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

int vsnprintf(char *str, size_t size, const char *format, va_list ap) {
    FILE f;
    int ret;
    f._flags = __S_WR | __S_STR;
    f._fd = -1;
    f._lock = 0;
    f._next = NULL;
    
    if (size > 0) {
        f._ptr = (unsigned char *)str;
        f._base = (unsigned char *)str;
        f._cnt = size - 1;
        f._bsize = size - 1;
    } else {
        // Safe length calculation mode: NULL buffer
        f._ptr = NULL;
        f._base = NULL;
        f._cnt = 0x7FFFFFFF; // Effectively infinite
        f._bsize = 0;
    }

    ret = vfprintf(&f, format, ap);
    
    if (size > 0) {
        if (f._ptr < f._base + size) {
            *f._ptr = '\0';
        } else {
            str[size - 1] = '\0';
        }
    }
    
    __stdio_remove_file(&f);

    return ret;
}
