#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <unistd.h>

#if defined(__GNUC__) || defined(__clang__)
#define UNREACHABLE __builtin_unreachable();
#pragma clang diagnostic ignored "-Wunknown-warning-option"
#pragma clang diagnostic ignored "-Wreserved-identifier"
#else
#define UNREACHABLE
#endif /* defined(__GNUC__) || defined(__clang__) */

void _call_atexit_handlers(void);

NORETURN void exit(int status) {
    _call_atexit_handlers();
    fflush(NULL);
    _exit(status);
    UNREACHABLE
}
