#include <stddef.h>
#include <unistd.h>
#include <sys/syscall.h>

#if !defined(SYS_exit) && defined(__NR_exit)
# define SYS_exit __NR_exit
#elif !defined(SYS_exit)
# error Please include proper syscall.h. Current one should support armv7 linux and ios
#endif

#if defined(__clang__) || defined(__GNUC__)
# define NORETURN __attribute__((noreturn))
# define UNREACHABLE __builtin_unreachable();
#endif

NORETURN void exit(int status) {
    syscall1(SYS_exit, status);
    UNREACHABLE
}
