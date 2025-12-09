#include <stdio.h>
#include <time.h>

static unsigned long int next_seed = 1;

void srand(unsigned int seed) {
    next_seed = (seed == 0) ? 1 : seed;
}

int rand(void) {
    const long a = 16807;
    const long m = 2147483647; // 0x7fffffff
    next_seed = (next_seed * a) % m;
    return (int)next_seed;
}
