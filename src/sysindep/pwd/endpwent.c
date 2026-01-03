#include <stdio.h>
#include <pwd.h>

extern FILE *pwd_file;

void endpwent(void) {
    if (pwd_file) {
        fclose(pwd_file);
        pwd_file = NULL;
    }
}
