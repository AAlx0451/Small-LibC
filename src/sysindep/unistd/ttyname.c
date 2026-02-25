#include <dirent.h>
#include <errno.h>
#include <limits.h>
#include <stddef.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

char *ttyname(int fd) {
    struct stat fs, ts;
    struct dirent *de;
    DIR *d;
    static char buf[PATH_MAX];
    size_t name_len;

    if(!isatty(fd)) {
        return NULL;
    }

    if(fstat(fd, &fs) < 0) {
        return NULL;
    }

    if(!S_ISCHR(fs.st_mode)) {
        errno = ENOTTY;
        return NULL;
    }

    d = opendir("/dev");
    if(!d) {
        return NULL;
    }

    while((de = readdir(d)) != NULL) {
        if(de->d_name[0] == '.' &&
           (de->d_name[1] == '\0' || (de->d_name[1] == '.' && de->d_name[2] == '\0'))) {
            continue;
        }

        name_len = strlen(de->d_name);
        if(5 + name_len + 1 > PATH_MAX) {
            continue;
        }

        strcpy(buf, "/dev/");
        strcat(buf, de->d_name);
        if(stat(buf, &ts) < 0) {
            continue;
        }

        if(ts.st_ino == fs.st_ino && ts.st_dev == fs.st_dev) {
            closedir(d);
            return buf;
        }
    }

    closedir(d);
    errno = ENOTTY;
    return NULL;
}
