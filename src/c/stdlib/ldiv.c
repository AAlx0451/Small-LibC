#include <stdlib.h>

ldiv_t ldiv(long numer, long denom) {
	ldiv_t res;
	res.quot = numer / denom;
	res.rem = numer % denom;
	return res;
}
