#if defined(__APPLE__) && defined(__arm__)

# ifndef SYS_TYPES_H
# define SYS_TYPES_H

#  ifndef NULL
#   define NULL ((void *)0)
#  endif

# include<stdint.h>

typedef int pid_t;
typedef unsigned short mode_t;
typedef int64_t off_t;
typedef long time_t;
typedef int32_t suseconds_t;
typedef unsigned int id_t;
typedef unsigned int uid_t;
typedef long ssize_t;
typedef unsigned long clock_t;

# endif /* SYS_TYPES_H */

#else
# error Unsupported OS
#endif

