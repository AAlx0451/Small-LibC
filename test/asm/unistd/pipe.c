#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define RED(txt)   "\033[0;31m" txt "\033[0m"
#define GREEN(txt) "\033[0;32m" txt "\033[0m"

void pipe_test()
{
    int fd[2];
    char buf[64];
    ssize_t nbytes;

    if (pipe(fd) == 0 && fd[0] >= 0 && fd[1] >= 0 && fd[0] != fd[1]) {
        printf(GREEN("[PASS]") " pipe() creation and valid distinct FDs\n");
    } else {
        printf(RED("[FAIL]") " pipe() creation\n");
        return;
    }

    close(fd[0]);
    close(fd[1]);

    if (pipe(fd) == 0) {
        const char *msg = "hello pipe";
        write(fd[1], msg, strlen(msg) + 1);

        memset(buf, 0, sizeof(buf));
        nbytes = read(fd[0], buf, sizeof(buf));

        if (nbytes == (ssize_t)(strlen(msg) + 1) && strcmp(buf, msg) == 0)
            printf(GREEN("[PASS]") " pipe() basic write and read\n");
        else
            printf(RED("[FAIL]") " pipe() basic write and read\n");

        close(fd[0]);
        close(fd[1]);
    }

    if (pipe(fd) == 0) {
        close(fd[1]);
        nbytes = read(fd[0], buf, sizeof(buf));
        if (nbytes == 0)
            printf(GREEN("[PASS]") " pipe() read returns 0 when write-end is closed\n");
        else
            printf(RED("[FAIL]") " pipe() read returns 0 when write-end is closed (returned %zd)\n",
                   nbytes);

        close(fd[0]);
    }

    if (pipe(fd) == 0) {
        void (*old_handler)(int) = signal(SIGPIPE, SIG_IGN);
        close(fd[0]);
        errno = 0;
        nbytes = write(fd[1], "test", 4);

        if (nbytes == -1 && errno == EPIPE)
            printf(GREEN("[PASS]") " pipe() write returns -1 and EPIPE when read-end is closed\n");
        else
            printf(RED("[FAIL]") " pipe() write returns -1 and EPIPE when read-end is closed\n");

        close(fd[1]);
        signal(SIGPIPE, old_handler);
    }

    if (pipe(fd) == 0) {
        pid_t pid = fork();

        if (pid == 0) {
            close(fd[0]);
            const char *ipc_msg = "IPC_DATA";
            write(fd[1], ipc_msg, strlen(ipc_msg) + 1);
            close(fd[1]);
            _exit(0);
        } else if (pid > 0) {
            close(fd[1]);
            memset(buf, 0, sizeof(buf));
            nbytes = read(fd[0], buf, sizeof(buf));
            close(fd[0]);
            waitpid(pid, NULL, 0);

            if (nbytes > 0 && strcmp(buf, "IPC_DATA") == 0)
                printf(GREEN("[PASS]") " pipe() inter-process communication\n");
            else
                printf(RED("[FAIL]") " pipe() inter-process communication\n");
        } else {
            printf("[SKIP]"
                   " pipe() inter-process communication (fork() failed)\n");
        }
    }
}
