#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void abort(void) {
    raise(SIGABRT);
    _exit(127);
    while (1) {}
}
