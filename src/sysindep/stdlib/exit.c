#include <stddef.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <stdio.h>
#include <stdlib.h>

# define UNREACHABLE __builtin_unreachable();

NORETURN void exit(int status) {
	_call_atexit_handlers();
	fflush(NULL);
	_exit(status);
	UNREACHABLE
}
