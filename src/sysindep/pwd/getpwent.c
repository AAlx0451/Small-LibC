#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

extern FILE *pwd_file;
static struct passwd p_entry;
static char p_buffer[1024];

struct passwd *getpwent(void) {
    char *p, *end;

    if(pwd_file == NULL) {
        if((pwd_file = fopen("/etc/passwd", "r")) == NULL) {
            return NULL;
        }
    }

    if(fgets(p_buffer, sizeof(p_buffer), pwd_file) == NULL) {
        return NULL;
    }

    p = p_buffer;

    end = strchr(p, ':');
    if(!end)
        return NULL;
    *end = '\0';
    p_entry.pw_name = p;
    p = end + 1;

    end = strchr(p, ':');
    if(!end)
        return NULL;
    *end = '\0';
    p_entry.pw_passwd = p;
    p = end + 1;

    end = strchr(p, ':');
    if(!end)
        return NULL;
    *end = '\0';
    p_entry.pw_uid = (uid_t)atoi(p);
    p = end + 1;

    end = strchr(p, ':');
    if(!end)
        return NULL;
    *end = '\0';
    p_entry.pw_gid = (gid_t)atoi(p);
    p = end + 1;

    end = strchr(p, ':');
    if(!end)
        return NULL;
    *end = '\0';
    p_entry.pw_gecos = p;
    p = end + 1;

    end = strchr(p, ':');
    if(!end)
        return NULL;
    *end = '\0';
    p_entry.pw_dir = p;
    p = end + 1;

    end = strchr(p, '\n');
    if(end)
        *end = '\0';
    p_entry.pw_shell = p;

    return &p_entry;
}
