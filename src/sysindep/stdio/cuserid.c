#include <pwd.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

char *cuserid(char *s) {
    struct passwd *pw;
    static char buf[L_cuserid];

    pw = getpwuid(geteuid());
    if(!pw) {
        if(s)
            s[0] = '\0';
        return s ? s : "";
    }

    if(s) {
        return strncpy(s, pw->pw_name, L_cuserid - 1)[L_cuserid - 1] = '\0', s;
    }

    return strncpy(buf, pw->pw_name, L_cuserid - 1)[L_cuserid - 1] = '\0', buf;
}
