#include <stddef.h>

size_t strxfrm(char *dest, const char *src, size_t n) {
    size_t len = 0;
    const char *s = src;
    while (*s++) len++;

    if (n > 0) {
        size_t copylen = (len < n) ? len : n - 1;
        size_t i;
        for (i = 0; i < copylen; i++)
            dest[i] = src[i];
        dest[i] = '\0';
    }
    return len;
}
