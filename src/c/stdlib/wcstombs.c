#include <stdlib.h>

size_t wcstombs(char *s, const wchar_t *pwcs, size_t n) {
	size_t i = 0;

	while (i < n) {
		if (*pwcs == 0) {
			s[i] = '\0';
			return i;
		}
		s[i] = (char)*pwcs;
		pwcs++;
		i++;
	}

	if (n > 0) {
	}

	return n;
}
