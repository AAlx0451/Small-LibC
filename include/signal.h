#ifndef SIGNAL_H
#define SIGNAL_H

#include <sys/types.h>
#include <features.h>

typedef int sig_atomic_t;

#if !defined(_ANSI) && (defined(_DARWIN_C_SOURCE) || defined(_POSIX_C_SOURCE) || defined(_XOPEN_SOURCE))
typedef int sigset_t;
#endif

#if defined(_DARWIN_C_SOURCE)
typedef void (*sig_t)(int);
#endif

#define SIG_DFL ((void (*)(int))0)
#define SIG_IGN ((void (*)(int))1)
#define SIG_ERR ((void (*)(int))-1)

#define SIGINT    2
#define SIGILL    4
#define SIGABRT   6
#define SIGFPE    8
#define SIGSEGV   11
#define SIGTERM   15

#if !defined(_ANSI) && (defined(_DARWIN_C_SOURCE) || defined(_POSIX_C_SOURCE) || defined(_XOPEN_SOURCE))
#define SIGHUP    1
#define SIGQUIT   3
#define SIGKILL   9
#define SIGPIPE   13
#define SIGALRM   14
#define SIGCHLD   20
#define SIGCONT   19
#define SIGSTOP   17
#define SIGTSTP   18
#define SIGTTIN   21
#define SIGTTOU   22
#define SIGUSR1   30
#define SIGUSR2   31
#endif

#if !defined(_ANSI) && (defined(_DARWIN_C_SOURCE) || (defined(_XOPEN_SOURCE) && _XOPEN_SOURCE >= 500) || (defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 200112L))
#define SIGTRAP   5
#define SIGBUS    10
#define SIGSYS    12
#define SIGURG    16
#define SIGXCPU   24
#define SIGXFSZ   25
#define SIGVTALRM 26
#define SIGPROF   27
#endif

#if defined(_DARWIN_C_SOURCE)
#define SIGIO     23
#define SIGWINCH  28
#define SIGINFO   29
#endif

#if !defined(_ANSI) && (defined(_DARWIN_C_SOURCE) || defined(_POSIX_C_SOURCE) || defined(_XOPEN_SOURCE))
#define NSIG      32
#endif

#if !defined(_ANSI) && (defined(_DARWIN_C_SOURCE) || defined(_POSIX_C_SOURCE) || defined(_XOPEN_SOURCE))
#define SA_NOCLDSTOP 0x0008

#if defined(_DARWIN_C_SOURCE) || (defined(_XOPEN_SOURCE) && _XOPEN_SOURCE >= 500) || (defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 200112L)
#define SA_ONSTACK   0x0001
#define SA_RESTART   0x0002
#define SA_RESETHAND 0x0004
#define SA_NODEFER   0x0010
#define SA_NOCLDWAIT 0x0020
#define SA_SIGINFO   0x0040
#endif

#if defined(_DARWIN_C_SOURCE)
#define SA_USERTRAMP 0x0100
#define UC_FLAVOR    30
#endif

#define SIG_BLOCK   1
#define SIG_UNBLOCK 2
#define SIG_SETMASK 3

struct sigaction {
    union {
        void (*__sa_handler)(int);
        void (*__sa_sigaction)(int, void *, void *);
    } __sigaction_u;
    sigset_t sa_mask;
    int      sa_flags;
};

#define sa_handler   __sigaction_u.__sa_handler
#define sa_sigaction __sigaction_u.__sa_sigaction
#endif

#if !defined(_ANSI) && (defined(_DARWIN_C_SOURCE) || (defined(_XOPEN_SOURCE) && _XOPEN_SOURCE >= 500) || (defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 199309L))
union sigval {
    int     sival_int;
    void    *sival_ptr;
};

typedef struct __siginfo {
    int     si_signo;
    int     si_errno;
    int     si_code;
    pid_t   si_pid;
    uid_t   si_uid;
    int     si_status;
    void    *si_addr;
    union sigval si_value;
    long    si_band;
    unsigned long   __pad[7];
} siginfo_t;
#endif

#if !defined(_ANSI) && (defined(_DARWIN_C_SOURCE) || defined(_POSIX_C_SOURCE) || defined(_XOPEN_SOURCE))
#define sigemptyset(set)    (*(set) = 0, 0)
#define sigfillset(set)     (*(set) = ~(sigset_t)0, 0)
#define sigaddset(set, sig) (*(set) |= (1 << ((sig) - 1)), 0)
#define sigdelset(set, sig) (*(set) &= ~(1 << ((sig) - 1)), 0)
#define sigismember(set, sig) ((*(set) & (1 << ((sig) - 1))) ? 1 : 0)
#endif

void (*signal(int sig, void (*func)(int)))(int);
int raise(int sig);

#if !defined(_ANSI) && (defined(_DARWIN_C_SOURCE) || defined(_POSIX_C_SOURCE) || defined(_XOPEN_SOURCE))
int sigaction(int sig, const struct sigaction *act, struct sigaction *oact);
int kill(pid_t pid, int sig);
int sigprocmask(int how, const sigset_t *set, sigset_t *oldset);
int sigsuspend(const sigset_t *sigmask);
int sigpending(sigset_t *set);
#endif

#if defined(_DARWIN_C_SOURCE)
int sigreturn(void *uctx, int infostyle);
#endif

#endif
