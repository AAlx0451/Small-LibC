#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

size_t confstr(int name, char *buf, size_t len)
{
    const char *val;
    size_t vlen;

    switch (name) {
    case _CS_PATH:
        val = getenv("PATH");
        if (val == NULL) {
            val = "/bin:/usr/bin";
        }
        break;
    default:
        errno = EINVAL;
        return 0;
    }

    vlen = strlen(val) + 1;

    if (len != 0 && buf != NULL) {
        size_t copy_len = (vlen < len) ? vlen : len;
        memcpy(buf, val, copy_len - 1);
        buf[copy_len - 1] = '\0';
    }

    return vlen;
}
