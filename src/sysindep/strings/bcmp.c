#include <strings.h>

int bcmp(const void *s1, const void *s2, size_t n) {
    const unsigned char *p1 = (const unsigned char *)s1;
    const unsigned char *p2 = (const unsigned char *)s2;
    while(n--) {
        if(*p1 != *p2) {
            return 1;
        }
        p1++;
        p2++;
    }
    return 0;
}
