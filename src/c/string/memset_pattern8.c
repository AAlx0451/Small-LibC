#include <stddef.h>
#include <string.h>

void memset_pattern8(void *b, const void *pattern8, size_t len)
{
    unsigned char *p = (unsigned char *)b;
    const unsigned char *pat = (const unsigned char *)pattern8;

    while (len >= 8) {
        memcpy(p, pat, 8);
        p += 8;
        len -= 8;
    }

    if (len > 0) {
        memcpy(p, pat, len);
    }
}
