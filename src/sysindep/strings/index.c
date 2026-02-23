#include <stdint.h>
#include <strings.h>

char *index(const char *s, int c) {
    while(*s) {
        if(*s == (char)c) {
            return (char *)(uintptr_t)s;
        }
        s++;
    }
    if(c == '\0') {
        return (char *)(uintptr_t)s;
    }
    return NULL;
}
