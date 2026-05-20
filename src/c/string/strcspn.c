#include <stddef.h>

size_t strcspn(const char *s, const char *reject)
{
    const char *p = s;
    const char *r;
    while (*p) {
        for (r = reject; *r; r++) {
            if (*p == *r)
                return (size_t)(p - s);
        }
        p++;
    }
    return (size_t)(p - s);
}
