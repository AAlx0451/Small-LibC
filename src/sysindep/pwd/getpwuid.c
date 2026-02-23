#include <pwd.h>
#include <sys/types.h>

struct passwd *getpwuid(uid_t uid) {
    struct passwd *p;
    setpwent();
    while((p = getpwent()) != NULL) {
        if(p->pw_uid == uid) {
            break;
        }
    }
    endpwent();
    return p;
}
