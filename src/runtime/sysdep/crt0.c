#include <stdlib.h>
#include <stdio.h>

#define _Used __attribute__((used)) /* readability */

_Used int NXArgc;
_Used char **NXArgv;
_Used char **environ;
_Used char *__progname;

extern int main(int argc, char **argv, char **envp, char **apple);
void __stack_chk_guard_init(void);

static char *get_basename(const char *path) {
    const char *p;
    if (!path) return "unknown";
    p = path;
    while (*path) {
        if (*path++ == '/') p = path;
    }
    return (char *)p;
}

_Used void _c_startup(int argc, char **argv, char **envp) {
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
    __stack_chk_guard_init();
#endif /* defined at S-LibC's stdio */

// actually,  #ifdef SMALL_LIBC isn't required, because with LC_MAIN lazy binding we don't need crt with libSystem (Argc/v stuff will be initialized using libdyld/libSystem symbols), but I just want to think that my code is compatible (but it's not, lol). and that's aside from SMALL_LIBC being always defined because this lib is incompatible with libSystem, so it's literally useless. but it's my lib, so y not

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
