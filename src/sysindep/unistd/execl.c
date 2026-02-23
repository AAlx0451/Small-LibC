#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

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

    argv = (char **)malloc(sizeof(char *) * ((size_t)argc + 1U));
    if(argv == NULL) {
        return -1;
    }

    va_start(args, arg);
    i = 0;
    if(arg != NULL) {
        argv[i++] = (char *)(uintptr_t)arg;
        while((tmp = va_arg(args, const char *)) != NULL) {
            argv[i++] = (char *)(uintptr_t)tmp;
        }
    }
    argv[i] = NULL;
    va_end(args);

    result = execve(path, argv, environ);

    free(argv);
    return result;
}
