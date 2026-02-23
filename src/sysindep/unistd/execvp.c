#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

extern char **environ;

int execvp(const char *file, char *const argv[]) {
    char *path, *p, *n, *buf;
    size_t flen, plen;

    if(strchr(file, '/')) {
        return execve(file, argv, environ);
    }

    path = getenv("PATH");
    if(!path)
        path = "/bin:/usr/bin";

    flen = strlen(file);
    buf = malloc(strlen(path) + flen + 2);
    if(!buf)
        return -1;

    for(p = path;; p = n) {
        n = strchr(p, ':');
        plen = n ? (size_t)(n - p) : strlen(p);

        memcpy(buf, p, plen);
        buf[plen] = '/';
        memcpy(buf + plen + 1, file, flen + 1);

        execve(buf, argv, environ);

        if(errno != ENOENT && errno != ENOTDIR) {
            free(buf);
            return -1;
        }

        if(!n)
            break;
        n++;
    }

    free(buf);
    return -1;
}
