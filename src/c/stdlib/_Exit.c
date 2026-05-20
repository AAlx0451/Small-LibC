#include <stdlib.h>
#include <unistd.h>

__noreturn void _Exit(int status) { _exit(status); }
