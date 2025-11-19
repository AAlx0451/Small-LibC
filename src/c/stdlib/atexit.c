#include <stdlib.h>

#define _ATEXIT_MAX 32

void (*_atexit_funcs[_ATEXIT_MAX])(void);
int _atexit_count = 0;

int atexit(void (*func)(void)) {
	if (_atexit_count >= _ATEXIT_MAX) {
		return -1;
	}
	_atexit_funcs[_atexit_count++] = func;
	return 0;
}
