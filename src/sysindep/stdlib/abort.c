#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

void abort(void) {
    raise(SIGABRT);
    _exit(127);
}
