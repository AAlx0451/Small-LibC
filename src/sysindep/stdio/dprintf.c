#include <stdarg.h>
#include <stdio.h>
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

    if(len == -1) {
        return -1;
    }

    bytes_written = write(fd, buffer, len);
    free(buffer);

    return bytes_written;
}
