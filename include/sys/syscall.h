#ifndef SYS_SYSCALL_H
#define SYS_SYSCALL_H

#if defined(__APPLE__) && defined(__arm__)

# define SYS_exit 1
# define SYS_write 4


#elif

/* we should include your own syscall.h. place it at sys/msyscall.c and define -DMSYSCALL */

# ifdef MSYSCALL
#  include <sys/msyscall.h>
# elif defined (__clang__) || defined(__GNUC__) 
#  include_next <sys/syscall.h> /* works if you have sys/syscall.h at include path
				 * recommended way is just replacing this file
				 * with yours */
# endif
#endif
#endif /* SYS_SYSCALL_H */
