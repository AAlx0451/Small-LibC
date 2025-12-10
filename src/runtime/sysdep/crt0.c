#include <stdlib.h>

int NXArgc;
char **NXArgv;
char **environ;
char *__progname;

__asm__(
    ".globl _NXArgc\n"
    ".desc _NXArgc, 0x10\n"
    ".globl _NXArgv\n"
    ".desc _NXArgv, 0x10\n"
    ".globl _environ\n"
    ".desc _environ, 0x10\n"
    ".globl ___progname\n"
    ".desc ___progname, 0x10\n"
);

extern int main(int argc, char **argv, char **envp, char **apple);

static char *get_basename(char *path) {
    char *p;
    if (!path) return "unknown";
    p = path;
    while (*path) {
        if (*path++ == '/') p = path;
    }
    return p;
}

void _c_startup(int argc, char **argv, char **envp) {
    char **ptr;
    char **apple;
    NXArgc = argc;
    NXArgv = argv;
    environ = envp;
    __progname = get_basename(argv[0]);
    ptr = envp;
    while (*ptr) {
        ptr++;
    }
    apple = ptr + 1;
    exit(main(argc, argv, envp, apple));
}

__asm__(
    ".text\n"
    ".arm\n"
    ".align 2\n"
    ".globl start\n"
    "start:\n"
    "    ldr r0, [sp]\n"
    "    add r1, sp, #4\n"
    "    add r2, r0, #1\n"
    "    add r2, r1, r2, lsl #2\n"
    "    bic sp, sp, #7\n"    
    "    bl __c_startup\n"
    "    trap\n"
);

