#include <stdlib.h>
#include <unistd.h>

NORETURN void _Exit(int status) {
    _exit(status);
}
