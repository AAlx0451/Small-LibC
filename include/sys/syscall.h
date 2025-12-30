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
#  define SYS_getpid 20
#  define SYS_access 33
#  define SYS_kill 37
#  define SYS_pipe 42
#  define SYS_sigaction 46
#  define SYS_sigprocmask 48
#  define SYS_ioctl 54
#  define SYS_execve 59
#  define SYS_munmap 73
#  define SYS_setitimer 83
#  define SYS_dup2 90
#  define SYS_select 93
#  define SYS_gettimeofday 116
#  define SYS_getrusage 117
#  define SYS_rename 128
#  define SYS_rmdir 137
#  define SYS_waitid 173
#  define SYS_sigreturn 184
#  define SYS_mmap 197
#  define SYS_lseek 199

# elif

/* we should include your own syscall.h. place it at sys/msyscall.c and define -DMSYSCALL */

#  ifdef MSYSCALL
#   include <sys/msyscall.h>
#  elif defined (__clang__) || defined(__GNUC__) 
#   include_next <sys/syscall.h> /* works if you have sys/syscall.h at include path
				 * recommended way is just replacing this file
				 * with yours */
#  endif
# endif
#endif /* SYS_SYSCALL_H */
