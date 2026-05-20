#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void _call_atexit_handlers(void);

__noreturn void exit(int status)
{
    _call_atexit_handlers();
    fflush(NULL);
    _exit(status);
}
