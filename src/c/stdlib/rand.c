#include <stdlib.h>

unsigned long _rand_seed = 1;

int rand(void) {
	_rand_seed = _rand_seed * 1103515245 + 12345;
	return (unsigned int)(_rand_seed / 65536) % 32768;
}
