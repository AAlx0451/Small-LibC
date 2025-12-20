#include <stdlib.h>

extern unsigned long _rand_seed;

void srand(unsigned int seed) {
	_rand_seed = seed;
}
