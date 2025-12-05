#if defined(__APPLE__) && defined(__arm__)

# ifndef SYS_TYPES_H
# define SYS_TYPES_H

#  ifndef NULL
#   define NULL ((void *)0)
#  endif

# include<stdint.h>

typedef int pid_t;
#  ifdef __APPLE__
typedef unsigned short mode_t;
#  else
typedef unsigned int mode_t;
#  endif
typedef long long __int64_t;
typedef __int64_t off_t;
typedef int __int32_t;
typedef long time_t;
typedef __int32_t suseconds_t;
typedef unsigned int id_t;
typedef unsigned int uid_t;
# endif /* SYS_TYPES_H */

#else
# error Unsupported OS
#endif

