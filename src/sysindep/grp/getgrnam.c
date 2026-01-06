#include <string.h>
#include "_grp.h"

struct group *getgrnam(const char *name) {
    struct group *grp;

    if (name == NULL) {
        return NULL;
    }

    setgrent();
    while ((grp = getgrent()) != NULL) {
        if (strcmp(grp->gr_name, name) == 0) {
            break;
        }
    }
    endgrent();
    
    return grp;
}
