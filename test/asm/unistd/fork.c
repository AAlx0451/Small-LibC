#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define RED(txt) "\033[0;31m" txt "\033[0m"
#define GREEN(txt) "\033[0;32m" txt "\033[0m"

void fork_test()
{
    pid_t pid;
    int status;
    pid = fork();

    if (pid < 0) {
        printf(RED("[FAIL]") " fork(): execution\n");
        return;
    } else if (pid == 0) {
        _exit(42);
    } else {
        waitpid(pid, &status, 0);
        if (WIFEXITED(status) && WEXITSTATUS(status) == 42)
            printf(GREEN("[PASS]") " fork(): execution and exit status retrieval\n");
        else
            printf(RED("[FAIL]") " fork(): execution and exit status retrieval\n");
    }

    volatile int shared_var = 100;
    pid = fork();

    if (pid == 0) {
        shared_var = 200;
        _exit(0);
    } else if (pid > 0) {
        waitpid(pid, NULL, 0);
        if (shared_var == 100)
            printf(GREEN("[PASS]") " fork(): memory isolation\n");
        else
            printf(RED("[FAIL]") " fork(): memory isolation\n");
    }

    int fd[2];

    if (pipe(fd) < 0) {
        printf(RED("[FAIL]") " pipe() creation for FD inheritance test\n");
    } else {
        pid_t parent_pid = getpid();
        pid = fork();

        if (pid == 0) {
            close(fd[0]);
            pid_t child_pid = getpid();
            pid_t child_ppid = getppid();
            write(fd[1], &child_pid, sizeof(pid_t));
            write(fd[1], &child_ppid, sizeof(pid_t));
            close(fd[1]);
            _exit(0);
        } else if (pid > 0) {
            close(fd[1]);
            pid_t reported_pid = 0, reported_ppid = 0;
            ssize_t b1 = read(fd[0], &reported_pid, sizeof(pid_t));
            ssize_t b2 = read(fd[0], &reported_ppid, sizeof(pid_t));
            close(fd[0]);
            waitpid(pid, NULL, 0);

            if (b1 == sizeof(pid_t) && b2 == sizeof(pid_t))
                printf(GREEN("[PASS]") " fork(): file descriptor inheritance\n");
            else
                printf(RED("[FAIL]") " fork()' file descriptor inheritance\n");

            if (reported_pid == pid)
                printf(
                    GREEN("[PASS]") " fork(): child getpid() matches parent fork() return value\n");
            else
                printf(
                    RED("[FAIL]") " fork(): child getpid() matches parent fork() return value\n");

            if (reported_ppid == parent_pid)
                printf(GREEN("[PASS]") " fork(): child getppid() matches parent getpid()\n");
            else
                printf(RED("[FAIL]") " fork(): child getppid() matches parent getpid()\n");
        }
    }

    pid = fork();

    if (pid == 0) {
        abort();
        _exit(1);
    } else if (pid > 0) {
        waitpid(pid, &status, 0);
        if (WIFSIGNALED(status) && WTERMSIG(status) == SIGABRT)
            printf(GREEN("[PASS]") " fork(): waitpid detects child killed by signal\n");
        else
            printf(RED("[FAIL]") " fork(): waitpid detects child killed by signal\n");
    }
}
