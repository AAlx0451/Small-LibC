#ifndef GRP_H
#define	GRP_H

#include <sys/types.h>
#include <stddef.h>

struct group {
    char    *gr_name;
    char    *gr_passwd;
    gid_t    gr_gid;
    char    **gr_mem;
};

struct group *getgrgid(gid_t id);
struct group *getgrnam(const char *name);
struct group *getgrent();
void setgrent();
void endgrent();

#endif // GRP_H
