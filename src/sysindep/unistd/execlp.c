#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>

int execlp(const char *file, const char *arg, ...) {
    va_list ap;
    int argc = 1;
    char **argv;
    int i;

    va_start(ap, arg);
    while(va_arg(ap, char *)) {
        argc++;
    }
    va_end(ap);

    argv = (char **)malloc((argc + 1) * sizeof(char *));
    if(!argv)
        return -1;

    argv[0] = (char *)arg;
    va_start(ap, arg);
    for(i = 1; i <= argc; i++) {
        argv[i] = va_arg(ap, char *);
    }
    va_end(ap);

    execvp(file, argv);
    free(argv);
    return -1;
}
