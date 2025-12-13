#include <stddef.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <stdio.h>
#include <stdlib.h>

#if defined(__GNUC__) || defined(__clang__)
# define UNREACHABLE __builtin_unreachable();
#else
# define UNREACHABLE
#endif /* defined(__GNUC__) || defined(__clang__) */

void _call_atexit_handlers(void);

NORETURN void exit(int status) {
	_call_atexit_handlers();
	fflush(NULL);
	_exit(status);
	UNREACHABLE
}
