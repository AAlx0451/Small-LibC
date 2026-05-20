#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#pragma clang diagnostic ignored "-Wunknown-warning-option"
#pragma clang diagnostic ignored "-Wreserved-identifier"

extern int NXArgc;
extern char **NXArgv;
extern char *__progname;
__used __noreturn void _c_startup(int argc, char **argv, char **envp);

__used int NXArgc;
__used char **NXArgv;
__used char **environ;
__used char *__progname;

extern int main(int argc, char **argv, char **envp, char **apple);
void __stack_chk_guard_init(void);

static char *get_basename(const char *path)
{
    const char *p;
    if (!path)
        return "unknown";
    p = path;
    while (*path) {
        if (*path++ == '/')
            p = path;
    }
    return __deconst(char *, p);
}

__used __noreturn void _c_startup(int argc, char **argv, char **envp)
{
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

    __stdio_init();
    __stack_chk_guard_init();

    exit(main(argc, argv, envp, apple));
}

__asm__(".text\n"
        ".arm\n"
        ".align 2\n"
        ".globl start\n"
        "start:\n"
        "    ldr r0, [sp]\n"           /* r0 = argc */
        "    add r1, sp, #4\n"         /* r1 = argv */
        "    add r2, r0, #1\n"         /* r2 = argc + 1 */
        "    add r2, r1, r2, lsl #2\n" /* r2 = argv + (argc + 1) * 4 */
        "    bic sp, sp, #7\n"
        "    bl __c_startup\n"
        "    trap\n");
