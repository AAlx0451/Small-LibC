#include <ctype.h>
#include <stdlib.h>

int atoi(const char *nptr) { return (int)strtol(nptr, (char **)NULL, 10); }
