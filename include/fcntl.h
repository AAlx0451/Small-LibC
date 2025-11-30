#if defined(__APPLE__) && defined(__arm__)

# ifndef FCNTL_H
# define FCNTL_H

#  define O_RDONLY 0x0000
#  define O_WRONLY 0x0001
#  define O_RDWR 0x0002
#  define O_ACCMODE 0x0003
#  define O_CREAT 0x0200
#  define O_TRUNC 0x0400

# endif

#else

# error "Unsupported OS"

#endif
