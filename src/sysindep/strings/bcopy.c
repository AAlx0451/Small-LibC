#include <strings.h>

void bcopy(const void *src, void *dest, size_t n) {
    const char *s = (const char *)src;
    char *d = (char *)dest;
    if(d < s) {
        while(n--) {
            *d++ = *s++;
        }
    } else {
        const char *lasts = s + (n - 1);
        char *lastd = d + (n - 1);
        while(n--) {
            *lastd-- = *lasts--;
        }
    }
}
