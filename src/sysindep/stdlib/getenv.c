#include <stddef.h>
#include <string.h>

extern char **environ;

char *getenv(const char *name) {
    if(name == NULL || environ == NULL) {
        return NULL;
    }

    size_t len = strlen(name);

    for(char **s = environ; *s != NULL; s++) {
        if(strncmp(name, *s, len) == 0 && (*s)[len] == '=') {
            return *s + len + 1;
        }
    }

    return NULL;
}
