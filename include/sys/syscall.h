#ifndef SYS_SYSCALL_H
#define SYS_SYSCALL_H

# if defined(__APPLE__) && defined(__arm__)

#  define SYS_exit 1
#  define SYS_fork 2
#  define SYS_read 3
#  define SYS_write 4
#  define SYS_open 5
#  define SYS_close 6
#  define SYS_wait4 7
/* no 8 on iOS */
#  define SYS_link 9
#  define SYS_unlink 10
#  define SYS_chdir 12
#  define SYS_chmod 15
#  define SYS_chown 16
#  define SYS_getpid 20
#  define SYS_setuid 23
#  define SYS_getuid 24
#  define SYS_geteuid 25
#  define SYS_access 33
#  define SYS_sync 36
#  define SYS_kill 37
#  define SYS_getppid 39
#  define SYS_dup 41
#  define SYS_pipe 42
#  define SYS_getegid 43
#  define SYS_sigaction 46
#  define SYS_getgid 47
#  define SYS_sigprocmask 48
#  define SYS_getlogin 49
#  define SYS_acct 51
#  define SYS_sigpending 52
#  define SYS_ioctl 54
#  define SYS_execve 59
#  define SYS_umask 60
#  define SYS_chroot 61
#  define SYS_munmap 73
#  define SYS_getgroups 79
#  define SYS_getpgrp 81
#  define SYS_setpgid 82
#  define SYS_setitimer 83
#  define SYS_dup2 90
#  define SYS_fcntl 92
#  define SYS_select 93
#  define SYS_setpriority 96
#  define SYS_getpriority 100
#  define SYS_sigsuspend 111
#  define SYS_gettimeofday 116
#  define SYS_getrusage 117
#  define SYS_settimeofday 122
#  define SYS_fchown 123
#  define SYS_rename 128
#  define SYS_mkfifo 132
#  define SYS_mkdir 136
#  define SYS_utimes 138
#  define SYS_rmdir 137
#  define SYS_setsid 147
#  define SYS_waitid 173
#  define SYS_setgid 181
#  define SYS_sigreturn 184
#  define SYS_stat 188
#  define SYS_fstat 189
#  define SYS_lstat 190
#  define SYS_pathconf 191
#  define SYS_fpathconf 192
#  define SYS_mmap 197
#  define SYS_lseek 199
#  define SYS___sysctl 202
#  define SYS_getsid 310
#  define SYS_getdirentries64 344

# endif

#endif /* SYS_SYSCALL_H */

