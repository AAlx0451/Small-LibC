#ifndef SYS_WAIT_H
# define SYS_WAIT_H

# include<sys/resource.h>

typedef enum {
    P_ALL,
    P_PID,
    P_PGID
} idtype_t;

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

# define w_termsig    w_T.w_Termsig
# define w_coredump   w_T.w_Coredump
# define w_retcode    w_T.w_Retcode
# define w_stopval    w_S.w_Stopval
# define w_stopsig    w_S.w_Stopsig

# define WNOHANG      0x00000001
# define WUNTRACED    0x00000002
# define WEXITED      0x00000004
# define WSTOPPED     0x00000008
# define WCONTINUED   0x00000010
# define WNOWAIT      0x00000020

# define WAIT_ANY     (-1)
# define WAIT_MYPGRP  0

# define _W_INT(w)      (*(int *)&(w))
# define _WSTATUS(x)    (_W_INT(x) & 0177)
# define _WSTOPPED      0177
# define WCOREFLAG      0200

# define WIFEXITED(x)     (_WSTATUS(x) == 0)
# define WIFSTOPPED(x)    (_WSTATUS(x) == _WSTOPPED && WSTOPSIG(x) != 0x13)
# define WIFSIGNALED(x)   (_WSTATUS(x) != _WSTOPPED && _WSTATUS(x) != 0)
# define WIFCONTINUED(x)  (_WSTATUS(x) == _WSTOPPED && WSTOPSIG(x) == 0x13)

# define WEXITSTATUS(x)   (_W_INT(x) >> 8)
# define WTERMSIG(x)      (_WSTATUS(x))
# define WSTOPSIG(x)      (_W_INT(x) >> 8)
# define WCOREDUMP(x)     (_W_INT(x) & WCOREFLAG)

# define W_EXITCODE(ret, sig)   ((ret) << 8 | (sig))
# define W_STOPCODE(sig)        ((sig) << 8 | _WSTOPPED)

pid_t wait(int *wstatus);
pid_t waitpid(pid_t pid, int *status, int options);
pid_t wait3(int *status, int options, struct rusage *rusage);
pid_t wait4(pid_t pid, int *status, int options, struct rusage *rusage);

#endif
