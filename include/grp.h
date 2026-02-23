#ifndef GRP_H
#define	GRP_H

#include <features.h>

#if !defined(_ANSI) && (defined(_POSIX_C_SOURCE) || defined(_XOPEN_SOURCE))

#include <sys/types.h>
#include <stddef.h>

struct group {
    char *gr_name;
    char *gr_passwd;
    gid_t gr_gid;
    char **gr_mem;
};

struct group *getgrgid(gid_t id);
struct group *getgrnam(const char *name);

# if defined(_XOPEN_SOURCE) || (defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 200112L)
struct group *getgrent();
void setgrent();
void endgrent();
# endif /* _XOPEN_SOURCE || (_POSIX_C_SOURCE && _POSIX_C_SOURCE >= 200112) */

#endif /* !_ANSI && (_POSIX_C_SOURCE || _XOPEN_SOURCE) */

#endif /* !GRP_H */
