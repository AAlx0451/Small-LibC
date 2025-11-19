#include <stddef.h>

size_t strcspn(const char *s, const char *reject) {
    const char *p = s;
    const char *r;
    while (*p) {
        for (r = reject; *r; r++) {
            if (*p == *r)
                return p - s;
        }
        p++;
    }
    return p - s;
}
