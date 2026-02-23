#include <pwd.h>
#include <stdio.h>

FILE *pwd_file = NULL;

void setpwent(void) {
    if(pwd_file) {
        rewind(pwd_file);
    } else {
        pwd_file = fopen("/etc/passwd", "r");
    }
}
