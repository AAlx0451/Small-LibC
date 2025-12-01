#if defined(__APPLE__) && defined(__arm__)

# ifndef SYS_TYPES_H
# define SYS_TYPES_H

#  ifndef NULL
#   define NULL ((void *)0)
#  endif

typedef int pid_t;
typedef int mode_t;
typedef long long __int64_t;
typedef __int64_t off_t;

# endif /* SYS_TYPES_H */

#else
# error Unsupported OS
#endif

