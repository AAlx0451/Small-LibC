#include <stdlib.h>
#include <ctype.h>

int atoi(const char *nptr) {
	int res = 0;
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
