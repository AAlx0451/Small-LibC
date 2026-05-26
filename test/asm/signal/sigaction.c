#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define RED(txt)   "\033[0;31m" txt "\033[0m"
#define GREEN(txt) "\033[0;32m" txt "\033[0m"

static volatile int caught_sigusr1 = 0;
static volatile int siginfo_signo = 0;
static volatile pid_t siginfo_pid = 0;

static void dummy_handler(int sig)
{
    if (sig == SIGUSR1)
        caught_sigusr1++;
}

static void info_handler(int sig, siginfo_t *info, __attribute__((unused)) void *ucontext)
{
    if (sig == SIGUSR1 && info != NULL) {
        siginfo_signo = info->si_signo;
        siginfo_pid = info->si_pid;
    }
}

void sigaction_test()
{
    struct sigaction act, oldact;
    int status;
    pid_t pid;
    act.sa_handler = dummy_handler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    errno = 0;

    if (sigaction(-1, &act, NULL) == -1 && errno == EINVAL)
        printf(GREEN("[PASS]") " sigaction fails with EINVAL on negative signum\n");
    else
        printf(RED("[FAIL]") " sigaction fails with EINVAL on negative signum\n");

    if (sigaction(SIGKILL, &act, NULL) == -1 && errno == EINVAL)
        printf(GREEN("[PASS]") " sigaction prevents catching SIGKILL\n");
    else
        printf(RED("[FAIL]") " sigaction prevents catching SIGKILL\n");

    if (sigaction(SIGSTOP, &act, NULL) == -1 && errno == EINVAL)
        printf(GREEN("[PASS]") " sigaction prevents catching SIGSTOP\n");
    else
        printf(RED("[FAIL]") " sigaction prevents catching SIGSTOP\n");

    caught_sigusr1 = 0;
    if (sigaction(SIGUSR1, &act, NULL) == 0) {
        raise(SIGUSR1);
        if (caught_sigusr1 == 1)
            printf(GREEN("[PASS]") " custom signal handler installed and triggered\n");
        else
            printf(RED("[FAIL]") " custom signal handler installed and triggered\n");
    } else {
        printf(RED("[FAIL]") " custom signal handler installed and triggered (setup failed)\n");
    }

    if (sigaction(SIGUSR1, NULL, &oldact) == 0 && oldact.sa_handler == dummy_handler)
        printf(GREEN("[PASS]") " sigaction reads previous handler correctly\n");
    else
        printf(RED("[FAIL]") " sigaction reads previous handler correctly\n");

    act.sa_handler = SIG_IGN;
    caught_sigusr1 = 0;
    if (sigaction(SIGUSR1, &act, NULL) == 0) {
        raise(SIGUSR1);
        if (caught_sigusr1 == 0)
            printf(GREEN("[PASS]") " SIG_IGN correctly ignores signal\n");
        else
            printf(RED("[FAIL]") " SIG_IGN correctly ignores signal\n");
    } else {
        printf(RED("[FAIL]") " SIG_IGN correctly ignores signal (setup failed)\n");
    }

    pid = fork();
    if (pid == 0) {
        act.sa_handler = dummy_handler;
        act.sa_flags = SA_RESETHAND;
        sigaction(SIGUSR1, &act, NULL);

        raise(SIGUSR1);
        raise(SIGUSR1);

        _exit(0);
    } else if (pid > 0) {
        waitpid(pid, &status, 0);
        if (WIFSIGNALED(status) && WTERMSIG(status) == SIGUSR1)
            printf(GREEN("[PASS]") " SA_RESETHAND restores default action after catching\n");
        else
            printf(RED("[FAIL]") " SA_RESETHAND restores default action after catching\n");
    } else {
        printf(RED("[FAIL]") " SA_RESETHAND test (fork failed)\n");
    }

    act.sa_sigaction = (void (*)(int, void *, void *))info_handler;
    act.sa_flags = SA_SIGINFO;
    if (sigaction(SIGUSR1, &act, NULL) == 0) {
        siginfo_signo = 0;
        siginfo_pid = 0;

        kill(getpid(), SIGUSR1);

        if (siginfo_signo == SIGUSR1 && siginfo_pid == getpid())
            printf(GREEN("[PASS]") " SA_SIGINFO passes basic siginfo_t fields correctly\n");
        else
            printf(RED("[FAIL]") " SA_SIGINFO passes basic siginfo_t fields correctly\n");
    } else {
        printf(
            RED("[FAIL]") " SA_SIGINFO passes basic siginfo_t fields correctly (setup failed)\n");
    }

    pid = fork();
    if (pid == 0) {
        int ret = sigaction(SIGUSR1, (struct sigaction *)0xdeadbeef, NULL);
        if (ret == -1 && errno == EFAULT)
            _exit(0);
        _exit(1);
    } else if (pid > 0) {
        waitpid(pid, &status, 0);
        if ((WIFEXITED(status) && WEXITSTATUS(status) == 0) || WIFSIGNALED(status))
            printf(GREEN(
                "[PASS]") " sigaction handles invalid pointer gracefully (EFAULT or SIGSEGV)\n");
        else
            printf(RED(
                "[FAIL]") " sigaction handles invalid pointer gracefully (EFAULT or SIGSEGV)\n");
    } else {
        printf(RED("[FAIL]") " invalid pointer test (fork failed)\n");
    }
}
