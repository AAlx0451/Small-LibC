#include <strings.h>

void bzero(void *s, size_t n) {
    unsigned char *p = (unsigned char *)s;
    while(n--) {
        *p++ = 0;
    }
}
