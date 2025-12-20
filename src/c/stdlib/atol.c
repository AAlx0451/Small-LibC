#include <stdlib.h>
#include <ctype.h>

long atol(const char *nptr) {
	long res = 0;
	int sign = 1;

	while (isspace(*nptr)) {
		nptr++;
	}

	if (*nptr == '+' || *nptr == '-') {
		if (*nptr == '-') {
			sign = -1;
		}
		nptr++;
	}

	while (isdigit(*nptr)) {
		res = res * 10 + (*nptr - '0');
		nptr++;
	}

	return sign * res;
}
