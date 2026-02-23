#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int system(const char *command) {
    pid_t pid;
    int status;
    const char *shell_path;

    if(command == NULL) {
        return 1;
    }

    shell_path = getenv("SHELL");
    if(shell_path == NULL) {
        shell_path = "/bin/sh";
    }

    pid = fork();
    if(pid < 0) {
        return -1;
    }

    if(pid == 0) {
        execl(shell_path, shell_path, "-c", command, (char *)NULL);
        _exit(127);
    }

    if(waitpid(pid, &status, 0) == -1) {
        return -1;
    }

    return status;
}
