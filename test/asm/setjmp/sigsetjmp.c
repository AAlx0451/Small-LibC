#include <setjmp.h>
#include <signal.h>
#include <stdio.h>

#define RED(txt)   "\033[0;31m" txt "\033[0m"
#define GREEN(txt) "\033[0;32m" txt "\033[0m"

static void rel(sigset_t *mask) { sigprocmask(SIG_UNBLOCK, mask, NULL); }

static void test(sigjmp_buf buf, int val, void (*f)(), void *param)
{
    f(param);
    siglongjmp(buf, val);
}

static sigjmp_buf test_buf;

static void handler(int sig)
{
    (void)sig;
    siglongjmp(test_buf, 10);
}

void sigsetjmp_test()
{
    {
        sigset_t mask;
        sigemptyset(&mask);
        sigaddset(&mask, SIGUSR1);

        if (sigprocmask(SIG_BLOCK, &mask, NULL) < 0)
            printf("[SKIP]"
                   " sigsetjmp: sigprocmask failed\n");
        else {
            sigjmp_buf buf;
            int status = sigsetjmp(buf, 1);

            if (!status)
                test(buf, 5, rel, &mask);
            if (status == 5) {
                sigset_t current_mask;
                sigprocmask(0, NULL, &current_mask);
                if (sigismember(&current_mask, SIGUSR1) == 1)
                    printf(GREEN("[PASS]") " siglongjmp: signal mask restored\n");
                else
                    printf(RED("[FAIL]") " siglongjmp: signal mask not restored\n");
            } else
                printf(RED("[FAIL]") " siglongjmp failed\n");
        }
    }

    {
        sigset_t mask;
        sigemptyset(&mask);
        sigaddset(&mask, SIGUSR1);

        if (sigprocmask(SIG_BLOCK, &mask, NULL) < 0)
            printf("[SKIP]"
                   " sigsetjmp: sigprocmask failed\n");
        else {
            sigjmp_buf buf;
            int status = sigsetjmp(buf, 0);

            if (!status)
                test(buf, 5, rel, &mask);
            if (status == 5) {
                sigset_t current_mask;
                sigprocmask(0, NULL, &current_mask);
                if (sigismember(&current_mask, SIGUSR1) == 1)
                    printf(RED("[FAIL]") " siglongjmp: signal mask restored\n");
                else
                    printf(GREEN("[PASS]") " siglongjmp: signal mask not restored\n");
            } else
                printf(RED("[FAIL]") " siglongjmp failed\n");
        }
    }

    {
        struct sigaction sa_new = {{0}, 0, 0};
        struct sigaction sa_old;
        sigset_t current_mask;

        sa_new.sa_handler = handler;
        sigemptyset(&sa_new.sa_mask);
        sa_new.sa_flags = 0;

        if (sigaction(SIGALRM, &sa_new, &sa_old) < 0) {
            printf("[SKIP] sigaction failed\n");
        } else {
            int status = sigsetjmp(test_buf, 1);

            if (status == 0) {
                raise(SIGALRM);
                printf(RED("[FAIL]") " siglongjmp: signal not delivered\n");
            } else if (status == 10) {
                sigprocmask(0, NULL, &current_mask);

                if (sigismember(&current_mask, SIGALRM) == 0)
                    printf(GREEN("[PASS]") " siglongjmp: mask restored after signal handler\n");
                else
                    printf(RED("[FAIL]") " siglongjmp: SIGALRM still blocked after jump\n");
            }

            sigaction(SIGALRM, &sa_old, NULL);
        }
    }
}
