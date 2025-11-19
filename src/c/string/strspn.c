#include <stddef.h>

size_t strspn(const char *s, const char *accept) {
    const char *p = s;
    const char *a;
    while (*p) {
        for (a = accept; *a; a++) {
            if (*p == *a)
                break;
        }
        if (*a == '\0')
            return p - s;
        p++;
    }
    return p - s;
}
