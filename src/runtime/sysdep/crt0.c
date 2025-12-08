#include <stddef.h>

int           NXArgc = 0;
char**        NXArgv = NULL;
char**        environ = NULL;
char*         __progname = NULL;

__asm__(
    ".desc _NXArgc, 0x10\n"
    ".desc _NXArgv, 0x10\n"
    ".desc _environ, 0x10\n"
    ".desc ___progname, 0x10"
);

__asm__(
    ".text\n"
    ".arm\n"
    ".align 2\n"
    ".globl start\n"
    "start:\n"
    "    ldr r0, [sp]\n"
    "    add r1, sp, #4\n"
    "    add r12, r0, #1\n"
    "    add r2, r1, r12, lsl #2\n"
    "    mov r3, r2\n"
    "1:\n"
    "    ldr r12, [r3], #4\n"
    "    cmp r12, #0\n"
    "    bne 1b\n"
    "    bic sp, sp, #7\n"
    "    bl _main\n"
    "    bl _exit\n"
    "    trap\n"
);
