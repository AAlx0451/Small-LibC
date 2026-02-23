#ifndef PWD_H
#define	PWD_H

#include <features.h>

#if !defined(_ANSI) && (defined(_POSIX_C_SOURCE) || defined(_XOPEN_SOURCE))

#include <sys/types.h>
#include <time.h>
#include <stddef.h>

struct passwd {
    char *pw_name;
    char *pw_passwd;
    uid_t pw_uid;
    gid_t pw_gid;
    time_t pw_change;
    char *pw_class;
    char *pw_gecos;
    char *pw_dir;
    char *pw_shell;
    time_t pw_expire;
};

struct passwd *getpwuid(uid_t);
struct passwd *getpwnam(const char *);

#if (defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 200112L) || defined(_XOPEN_SOURCE)
struct passwd *getpwent(void);
void setpwent(void);
void endpwent(void);
#endif /* (_POSIX_C_SOURCE && _POSIX_C_SOURCE >= 200112L) || (_XOPEN_SOURCE) */

#endif /* !_ANSI && (_POSIX_C_SOURCE || _XOPEN_SOURCE) */

#endif /* !PWD_H */
