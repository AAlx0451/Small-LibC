#include <stdlib.h>
#include <stdio.h>

int NXArgc;
char **NXArgv;
char **environ;
char *__progname;

__asm__(
    ".globl _NXArgc\n"
    ".desc _NXArgc, 0x10\n"   /* N_NO_DEAD_STRIP */
    ".globl _NXArgv\n"
    ".desc _NXArgv, 0x10\n"
    ".globl _environ\n"
    ".desc _environ, 0x10\n"
    ".globl ___progname\n"
    ".desc ___progname, 0x10\n"
);

extern int main(int argc, char **argv, char **envp, char **apple);

static char *get_basename(const char *path) {
    const char *p;
    if (!path) return "unknown";
    p = path;
    while (*path) {
        if (*path++ == '/') p = path;
    }
    return (char *)p;
}

__attribute__((used))
void _c_startup(int argc, char **argv, char **envp) {
    char **ptr;
    char **apple = NULL;
    NXArgc = argc;
    NXArgv = argv;
    environ = envp;   
    if (argv && argv[0]) {
        __progname = get_basename(argv[0]);
    } else {
        __progname = "unknown";
    }
    if (envp) {
        ptr = envp;
        while (*ptr) {
            ptr++;
        }
        apple = ptr + 1;
    }

#ifdef SMALL_LIBC
    __stdio_init();
#endif /* defined at S-LibC's stdio */

    exit(main(argc, argv, envp, apple));
}

__asm__(
    ".text\n"
    ".arm\n"
    ".align 2\n"
    ".globl start\n"
    "start:\n"
    "    ldr r0, [sp]\n"             /* r0 = argc */
    "    add r1, sp, #4\n"           /* r1 = argv */
    "    add r2, r0, #1\n"           /* r2 = argc + 1 */
    "    add r2, r1, r2, lsl #2\n"   /* r2 = argv + (argc + 1) * 4 */
    "    bic sp, sp, #7\n"
    "    bl __c_startup\n"
    "    trap\n" /* how could get here? exit() is broken(?) */
);
