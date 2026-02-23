#include "signal.h"
#include <stddef.h>

sig_t signal(int sig, sig_t func) {
    struct sigaction sa;
    struct sigaction osa;

    sa.sa_handler = func;
    sa.sa_mask = 0;
    sa.sa_flags = SA_RESTART;

    if(sigaction(sig, &sa, &osa) < 0) {
        return SIG_ERR;
    }

    return osa.sa_handler;
}
