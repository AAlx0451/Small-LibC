#include <strings.h>

int strncasecmp(const char *s1, const char *s2, size_t n) {
    const unsigned char *p1 = (const unsigned char *)s1;
    const unsigned char *p2 = (const unsigned char *)s2;
    int result;
    unsigned char c1, c2;

    if (n == 0)
        return 0;

    do {
        c1 = *p1++;
        c2 = *p2++;
        if (c1 >= 'A' && c1 <= 'Z') c1 += 32;
        if (c2 >= 'A' && c2 <= 'Z') c2 += 32;
        result = c1 - c2;
        if (result != 0)
            return result;
        if (c1 == 0)
            break;
    } while (--n != 0);

    return 0;
}
