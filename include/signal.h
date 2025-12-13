#ifndef SIGNAL_H
# define SIGNAL_H

# include<sys/types.h>

# define SIGABRT 6

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

typedef uint32_t sigset_t;

int kill(pid_t pid, int sig);
int raise(int sig);

#endif
