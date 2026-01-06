#include "_grp.h"

struct group *getgrgid(gid_t gid) {
    struct group *grp;

    setgrent();
    while ((grp = getgrent()) != NULL) {
        if (grp->gr_gid == gid) {
            break;
        }
    }
    endgrent();

    return grp;
}
