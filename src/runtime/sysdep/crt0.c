#include <stdlib.h>

int NXArgc;
char **NXArgv;
char **environ;
char *__progname;

extern int main(int argc, char **argv, char **envp);

void _c_startup(int argc, char **argv, char **envp) {
    NXArgc = argc;
    NXArgv = argv;
    environ = envp;
    __progname = (argv && argv[0]) ? argv[0] : "";
    exit(main(argc, argv, envp));
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
    "    add r2, r1, r2, lsl #2 \n"
    "    bic sp, sp, #7\n"
    "    bl __c_startup\n" 
    "    trap\n"
);
