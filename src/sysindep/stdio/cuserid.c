#include <pwd.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

char *cuserid(char *s) {
    struct passwd *pw;
    static char buf[L_cuserid];

    pw = getpwuid(geteuid());
    char *target = s ? s : buf;

    if(!pw) {
        target[0] = '\0';
        return s ? s : NULL;
    }

    strncpy(target, pw->pw_name, L_cuserid - 1);
    target[L_cuserid - 1] = '\0';

    return target;
}
