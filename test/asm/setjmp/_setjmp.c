#include <setjmp.h>
#include <signal.h>
#include <stdio.h>

#define RED(txt)   "\033[0;31m" txt "\033[0m"
#define GREEN(txt) "\033[0;32m" txt "\033[0m"

static void rel(sigset_t *mask) { sigprocmask(SIG_UNBLOCK, mask, NULL); }

static void test(jmp_buf buf, int val, void (*f)(), void *param)
{
    f(param);
    _longjmp(buf, val);
}

void _setjmp_test()
{
    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGUSR1);

    if (sigprocmask(SIG_BLOCK, &mask, NULL) < 0)
        printf("[SKIP]"
               " _setjmp: sigprocmask failed\n");
    else {
        jmp_buf buf;
        int status = _setjmp(buf);

        if (!status)
            test(buf, 5, rel, &mask);
        if (status == 5) {
            sigset_t current_mask;
            sigprocmask(0, NULL, &current_mask);
            if (sigismember(&current_mask, SIGUSR1) == 0)
                printf(GREEN("[PASS]") " _longjmp: signal mask not restored\n");
            else
                printf(RED("[FAIL]") " _longjmp: signal mask not restored\n");
        } else
            printf(RED("[FAIL]") " _longjmp failed\n");
    }
}
