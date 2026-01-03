#include <string.h>
#include <pwd.h>

struct passwd *getpwnam(const char *name) {
    struct passwd *p;
    setpwent();
    while ((p = getpwent()) != NULL) {
        if (strcmp(p->pw_name, name) == 0) {
            break;
        }
    }
    endpwent();
    return p;
}
