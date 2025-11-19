#include <stdlib.h>

int mbtowc(wchar_t *pwc, const char *s, size_t n) {
	if (s == NULL) {
		return 0;
	}
	if (n == 0) {
		return -1;
	}
	if (*s == '\0') {
		if (pwc != NULL) {
			*pwc = 0;
		}
		return 0;
	}
	if (pwc != NULL) {
		*pwc = (wchar_t)*s;
	}
	return 1;
}
