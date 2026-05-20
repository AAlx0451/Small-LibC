#include <stdint.h>

char *strrchr(const char *s, int c)
{
    const char *last = 0;
    do {
        if (*s == (char)c)
            last = s;
    } while (*s++);
    return (char *)(uintptr_t)last;
}
