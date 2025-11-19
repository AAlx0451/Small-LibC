#include <stdlib.h>

size_t mbstowcs(wchar_t *pwcs, const char *s, size_t n) {
	size_t i = 0;
	
	if (pwcs == NULL) {
		while (*s != '\0') {
			i++;
			s++;
		}
		return i;
	}

	while (i < n) {
		if (*s == '\0') {
			pwcs[i] = 0;
			return i;
		}
		pwcs[i] = (wchar_t)*s;
		s++;
		i++;
	}
	
	return n;
}
