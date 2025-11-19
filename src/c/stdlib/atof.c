#include <stdlib.h>

double atof(const char *nptr) {
	return strtod(nptr, (char **)NULL);
}
