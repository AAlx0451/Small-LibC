#include <unistd.h>
#include <sys/syscall.h>

#if defined(__clang__) || defined(__GNUC__)
# define UNREACHABLE __builtin_unreachable();
#else
# define UNREACHABLE
#endif

NORETURN void _exit(int status) {
    syscall(SYS_exit, status);
    UNREACHABLE
}
