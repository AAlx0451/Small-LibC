#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>

extern char **environ;

int execl(const char *path, const char *arg, ...) {
    va_list args;
    int argc;
    char **argv;
    const char *tmp;
    int i;
    int result;

    argc = 0;
    va_start(args, arg);
    if(arg != NULL) {
        argc++;
        while(va_arg(args, const char *) != NULL) {
            argc++;
        }
    }
    va_end(args);

    argv = (char **)malloc(sizeof(char *) * (argc + 1));
    if(argv == NULL) {
        return -1;
    }

    va_start(args, arg);
    i = 0;
    if(arg != NULL) {
        argv[i++] = (char *)arg;
        while((tmp = va_arg(args, const char *)) != NULL) {
            argv[i++] = (char *)tmp;
        }
    }
    argv[i] = NULL;
    va_end(args);

    result = execve(path, argv, environ);

    free(argv);
    return result;
}
