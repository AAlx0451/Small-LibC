#include <stdlib.h>

int mblen(const char *s, size_t n) {
	if (s == NULL) {
		return 0;
	}
	if (n == 0) {
		return -1;
	}
	if (*s == '\0') {
		return 0;
	}
	return 1;
}
