#include <stddef.h>
#include <string.h>

void memset_pattern4(void *b, const void *pattern4, size_t len)
{
    unsigned char *p = (unsigned char *)b;
    const unsigned char *pat = (const unsigned char *)pattern4;

    while (len >= 4) {
        memcpy(p, pat, 4);
        p += 4;
        len -= 4;
    }

    if (len > 0) {
        memcpy(p, pat, len);
    }
}
