#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

ssize_t getline(char **lineptr, size_t *n, FILE *stream) {
    if (lineptr == NULL || n == NULL || stream == NULL) {
        errno = EINVAL;
        return -1;
    }

    if (*lineptr == NULL || *n == 0) {
        *n = 128;
        *lineptr = malloc(*n);
        if (*lineptr == NULL) {
            return -1;
        }
    }

    size_t cur_len = 0;
    int c;

    while ((c = getc(stream)) != EOF) {
        if (cur_len + 1 >= *n) {
            size_t new_size = *n * 2;
            char *new_ptr = realloc(*lineptr, new_size);
            if (new_ptr == NULL) {
                return -1;
            }
            *lineptr = new_ptr;
            *n = new_size;
        }

        (*lineptr)[cur_len++] = (char)c;
        if (c == '\n') {
            break;
        }
    }

    if (c == EOF && cur_len == 0) {
        return -1;
    }

    (*lineptr)[cur_len] = '\0';
    return (ssize_t)cur_len;
}
