#include <stdlib.h>
#include <unistd.h>

extern void (*_atexit_funcs[])(void);
extern int _atexit_count;

void exit(int status) {
	while (_atexit_count > 0) {
		_atexit_funcs[--_atexit_count]();
	}
	/* fflush(NULL) */
	_exit(status & 0377);
}
