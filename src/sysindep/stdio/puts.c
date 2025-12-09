#include <unistd.h>
#include <string.h>
#include <stdio.h>

int puts(const char *s)
{
    size_t len = strlen(s);
    if (write(1, s, len) < 0) {
        return EOF;
    }

    if (write(1, "\n", 1) < 0) {
        return EOF;
    }
    return 0;
}
