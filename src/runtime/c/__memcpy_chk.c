#include <stddef.h>
#include "runtime.h"

void *__memcpy_chk(void *dest, const void *src, size_t len, size_t destlen) {
    if (len > destlen) {
        __chk_fail();
    }

    char *d = (char *)dest;
    const char *s = (const char *)src;
    for (size_t i = 0; i < len; i++) {
        d[i] = s[i];
    }
    return dest;
}
