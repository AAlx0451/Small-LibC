#ifndef SYS_WAIT_H
#define SYS_WAIT_H

#include <features.h>

#if !defined(_ANSI) && (defined(_POSIX_C_SOURCE) || defined(_XOPEN_SOURCE))

#include <sys/types.h>

#define WNOHANG 1
#define WUNTRACED 2

#define WIFEXITED(s) (((s) & 0xff) == 0)
#define WIFSTOPPED(s) (((s) & 0xff) == 0x7f)
#define WIFSIGNALED(s) (((s) & 0xff) != 0x7f && ((s) & 0xff) != 0)
#define WEXITSTATUS(s) (((s) >> 8) & 0xff)
#define WTERMSIG(s) ((s) & 0x7f)
#define WSTOPSIG(s) (((s) >> 8) & 0xff)

pid_t wait(int *stat_loc);
pid_t waitpid(pid_t pid, int *stat_loc, int options);

#if defined(_XOPEN_SOURCE) || defined(_GNU_SOURCE) || (defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 200112L)
#include <signal.h>

#define WEXITED 4
#define WSTOPPED 8
#define WCONTINUED 16
#define WNOWAIT 32

#define WIFCONTINUED(s) ((s) == 0xffff)

typedef enum {
    P_ALL,
    P_PID,
    P_PGID
} idtype_t;

int waitid(idtype_t idtype, id_t id, siginfo_t *infop, int options);
#endif /* _XOPEN_SOURCE || _GNU_SOURCE || (_POSIX_C_SOURCE && _POSIX_C_SOURCE >= 200112L) */

#if defined(_GNU_SOURCE) || defined(_DARWIN_C_SOURCE)
#include <sys/resource.h>

#define WAIT_ANY (-1)
#define WAIT_MYPGRP 0
#define WCOREFLAG 0200
#define WCOREDUMP(s) ((s) & WCOREFLAG)

union wait {
    int w_status;
    struct {
        unsigned int w_Termsig:7;
        unsigned int w_Coredump:1;
        unsigned int w_Retcode:8;
        unsigned int w_Filler:16;
    } w_T;
    struct {
        unsigned int w_Stopval:8;
        unsigned int w_Stopsig:8;
        unsigned int w_Filler:16;
    } w_S;
};

#define w_termsig w_T.w_Termsig
#define w_coredump w_T.w_Coredump
#define w_retcode w_T.w_Retcode
#define w_stopval w_S.w_Stopval
#define w_stopsig w_S.w_Stopsig

pid_t wait3(int *stat_loc, int options, struct rusage *rusage);
pid_t wait4(pid_t pid, int *stat_loc, int options, struct rusage *rusage);
#endif /* _GNU_SOURCE || _DARWIN_C_SOURCE || !_POSIX_C_SOURCE */

#endif /* !_ANSI && (_POSIX_C_SOURCE || _XOPEN_SOURCE) */

#endif /* !SYS_WAIT_H */
