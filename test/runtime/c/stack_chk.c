#include "runtime.h"
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define RED(txt) "\033[0;31m" txt "\033[0m"
#define GREEN(txt) "\033[0;32m" txt "\033[0m"

void stack_chk_test()
{
    char buf[10];
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        return;
    }

    if (pid == 0) {
        close(STDERR_FILENO);
        memset(buf, 'A', 100);
        printf(RED("[FAIL]") " process not killed on stack smashing");
    } else {
        int status;
        waitpid(pid, &status, 0);

        if (WIFSIGNALED(status)) {
            printf(GREEN("[PASS]") " process successfully killed after stack smash\n");
        }
    }
}
