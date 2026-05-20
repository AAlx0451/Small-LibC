#include <stddef.h>
#include <stdint.h>

size_t strspn(const char *s, const char *accept)
{
    const char *p = s;
    const char *a;
    while (*p) {
        for (a = accept; *a; a++) {
            if (*p == *a)
                break;
        }
        if (*a == '\0')
            return (size_t)(p - s);
        p++;
    }
    return (size_t)(p - s);
}
