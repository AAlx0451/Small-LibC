#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#pragma clang diagnostic ignored "-Wreserved-identifier"

/* global state for unique generation */
static volatile int _tmpnam_lock = 0;
static unsigned int _tmpnam_counter = 0;
static char _tmpnam_static_buf[L_tmpnam];

char *tmpnam(char *s) {
    char *p;
    unsigned int count;
    pid_t pid;
    int attempts;

    /* determine buffer (user-provided or internal static) */
    if(s == NULL) {
        p = _tmpnam_static_buf;
    } else {
        p = s;
    }

    pid = getpid();

    /* generation loop */
    for(attempts = 0; attempts < TMP_MAX; attempts++) {

        /* get unique counter value atomically */
        /* we lock ONLY the increment to ensure scalability */
        _spin_lock(&_tmpnam_lock);
        count = _tmpnam_counter++;
        _spin_unlock(&_tmpnam_lock);

        /* format the filename */
        /* /tmp/t<pid>.<counter> */
        sprintf(p, "/tmp/t%u.%u", (unsigned int)pid, count);

        /* check for existence using access() */
        if(access(p, F_OK) == -1) {
            /* ENOENT - name is free */
            if(errno == ENOENT) {
                return p;
            }
        }
    }

    /* failed to generate unique name after TMP_MAX attempts :( */
    return NULL;
}
