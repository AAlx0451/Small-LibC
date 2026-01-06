#if defined(__APPLE__) && defined(__arm__)

# ifndef FCNTL_H
# define FCNTL_H

/* CONSTANTS */
#  define O_RDONLY 0x0000
#  define O_WRONLY 0x0001
#  define O_RDWR 0x0002
#  define O_ACCMODE 0x0003
#  define O_APPEND 0x0008
#  define O_CREAT 0x0200
#  define O_TRUNC 0x0400
#  define O_EXCL 0x0800
#  define O_DIRECTORY 0x100000

# endif /* FCNTL_H */

#else

# error "Unsupported OS"

#endif
