#include <stddef.h>
#include <string.h>

void memset_pattern16(void *b, const void *pattern16, size_t len)
{
    unsigned char *p = (unsigned char *)b;
    const unsigned char *pat = (const unsigned char *)pattern16;

    while (len >= 16) {
        memcpy(p, pat, 16);
        p += 16;
        len -= 16;
    }

    if (len > 0) {
        memcpy(p, pat, len);
    }
}
