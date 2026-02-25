#include <sys/syscall.h>
#include <unistd.h>

#pragma clang diagnostic ignored "-Wunknown-warning-option"
#pragma clang diagnostic ignored "-Wreserved-identifier"

static char _login_name[64];
char *getlogin(void) {
    if(syscall(SYS_getlogin, (long)_login_name, sizeof(_login_name)) < 0) {
        return NULL;
    }
    return _login_name;
}
