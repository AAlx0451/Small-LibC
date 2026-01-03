#ifndef PWD_H
#define	PWD_H

#include <sys/types.h>
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
struct passwd *getpwent();
void setpwent();
void endpwent();

#endif // PWD_H
