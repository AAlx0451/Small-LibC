#include <stdlib.h>

#pragma clang diagnostic ignored "-Wunknown-warning-option"
#pragma clang diagnostic ignored "-Wreserved-identifier"

void _call_atexit_handlers(void);

#define ATEXIT_MAX 32

typedef void (*handler_func_t)(void);

static handler_func_t _atexit_handlers[ATEXIT_MAX];
static int _atexit_handler_count = 0;

int atexit(handler_func_t func) {
    if(_atexit_handler_count >= ATEXIT_MAX) {
        return 1;
    }
    _atexit_handlers[_atexit_handler_count++] = func;
    return 0;
}

void _call_atexit_handlers(void) {
    while(_atexit_handler_count > 0) {
        _atexit_handlers[--_atexit_handler_count]();
    }
}
