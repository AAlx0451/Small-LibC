#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

int execle(const char *path, const char *arg, ...) {
    va_list ap;
    int argc = 1;
    char **argv;
    char **envp;
    int i;

    va_start(ap, arg);
    while(va_arg(ap, char *)) {
        argc++;
    }
    envp = va_arg(ap, char **);
    va_end(ap);

    argv = (char **)malloc(((size_t)argc + 1) * sizeof(char *));
    if(!argv)
        return -1;

    argv[0] = (char *)(uintptr_t)arg;
    va_start(ap, arg);
    for(i = 1; i <= argc; i++) {
        argv[i] = va_arg(ap, char *);
    }
    va_end(ap);

    execve(path, argv, envp);
    free(argv);
    return -1;
}
