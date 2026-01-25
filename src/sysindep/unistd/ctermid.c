#include <stdio.h>
#include <string.h>

char *ctermid(char *s)
{
    static char default_buf[L_ctermid];
    if (!s) s = default_buf;
    strcpy(s, "/dev/tty");
    return s;
}
