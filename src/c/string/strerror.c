#include <stdio.h>

char *strerror(int errnum) {
    static char buf[64];

    if (errnum == 0)
        return "Success";

    sprintf(buf, "error %d", errnum);
    return buf;
}
