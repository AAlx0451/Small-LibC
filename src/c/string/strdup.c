#include <stddef.h>
#include <stdlib.h>

char *strdup(const char *s) {
    size_t len = 0;
    const char *p = s;
    while (*p++) len++;
    
    char *copy = (char *)malloc(len + 1);
    if (!copy)
        return 0;
    
    char *cp = copy;
    while ((*cp++ = *s++));
    
    return copy;
}
