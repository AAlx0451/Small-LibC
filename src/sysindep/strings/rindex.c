#include <stdint.h>
#include <strings.h>

char *rindex(const char *s, int c) {
    const char *last = NULL;
    do {
        if(*s == (char)c) {
            last = s;
        }
    } while(*s++);
    return (char *)(uintptr_t)last;
}
