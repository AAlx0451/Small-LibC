#include <unistd.h>
#include <sys/syscall.h>

#if defined(__clang__) || defined(__GNUC__)
# define UNREACHABLE __builtin_unreachable();
#endif

NORETURN void _exit(int status) {
    syscall1(SYS_exit, status);
    UNREACHABLE
}
