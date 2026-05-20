#include <sys/wait.h>
#include <unistd.h>

#define RED(txt) "\033[0;31m" txt "\033[0m"
#define GREEN(txt) "\033[0;32m" txt "\033[0m"

extern char **NXArgv;
extern int NXArgc;
extern char *__progname;

int static check(void *ptr)
{
    pid_t pid = fork();

    if (pid < 0) {
        return -1;
    }

    if (pid == 0) {
        if (ptr != NULL) {
            volatile char val = *(volatile char *)ptr;
            (void)val;
        } else {
            _exit(1);
        }
        _exit(0);
    } else {
        int status;
        waitpid(pid, &status, 0);

        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
            return 0;
        }
        return -1;
    }
}

void _c_startup_test()
{
    int argc = NXArgc;
    char **argv = NXArgv;

    if (argc >= 1)
        printf(GREEN("[PASS]") " argc is set correctly (%d)\n", argc);
    else
        printf(RED("[FAIL]") " argc is not set or set incorrectly (%d)\n", argc);

    if (argv) {
        if (check(argv) != -1 && check(*argv) != -1) /* argv[0] always set */
            printf(GREEN("[PASS]") " argv is set correctly (argv[0]: %s)\n", argv[0]);
        else
            printf(RED("[FAIL]") " argv is set incorrectly (%p)\n", argv);
    } else
        printf(RED("[FAIL]") " argv is not set");

    if (__progname) {
        if (check(__progname) != -1)
            printf(GREEN("[PASS]") " __progname is set correctly (%s)\n", __progname);
        else
            printf(RED("[FAIL]") " __progname is set incorrectly (%p)\n", __progname);
    } else
        printf(RED("[FAIL]") " __progname is not set");

    if (environ) {
        if (check(environ) != -1) /* environ[0] can be NULL, so no check(*environ) */
            printf(GREEN("[PASS]") " environ is set correctly\n");
        else
            printf(RED("[FAIL]") " environ is set incorrectly (%p)\n", environ);
    } else
        printf(RED("[FAIL]") " environ is not set");
}
