#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char *getenv(const char *name) {
    size_t len;
    char **s;
    if(name == NULL || environ == NULL) {
        return NULL;
    }

    len = strlen(name);

    for(s = environ; *s != NULL; s++) {
        if(strncmp(name, *s, len) == 0 && (*s)[len] == '=') {
            return *s + len + 1;
        }
    }

    return NULL;
}
