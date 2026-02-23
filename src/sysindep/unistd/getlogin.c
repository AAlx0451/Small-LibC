#include <sys/syscall.h>
#include <unistd.h>
static char _login_name[64];
char *getlogin(void) {
    if(syscall(SYS_getlogin, (long)_login_name, sizeof(_login_name)) < 0) {
        return (void *)0;
    }
    return _login_name;
}
