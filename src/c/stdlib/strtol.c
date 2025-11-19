#include <stdlib.h>
#include <ctype.h>
#include <limits.h>
#include <errno.h>

long strtol(const char *nptr, char **endptr, int base) {
	const char *s = nptr;
	unsigned long acc;
	int c;
	unsigned long cutoff;
	int neg = 0, any, cutlim;

	if (base < 0 || base == 1 || base > 36) {
		if (endptr != NULL) *endptr = (char *)nptr;
		errno = EINVAL;
		return 0;
	}

	while (isspace(*s)) s++;

	if (*s == '-') {
		neg = 1;
		s++;
	} else if (*s == '+') {
		s++;
	}

	if ((base == 0 || base == 16) && *s == '0' && (s[1] == 'x' || s[1] == 'X')) {
		s += 2;
		base = 16;
	}
	if (base == 0) {
		base = *s == '0' ? 8 : 10;
	}

	cutoff = neg ? -(unsigned long)LONG_MIN : LONG_MAX;
	cutlim = cutoff % (unsigned long)base;
	cutoff /= (unsigned long)base;

	for (acc = 0, any = 0;; c = *s++) {
		if (isdigit(c)) c -= '0';
		else if (isalpha(c)) c -= isupper(c) ? 'A' - 10 : 'a' - 10;
		else break;
		
		if (c >= base) break;
		
		if (any < 0 || acc > cutoff || (acc == cutoff && c > cutlim)) {
			any = -1;
		} else {
			any = 1;
			acc *= base;
			acc += c;
		}
	}

	if (any < 0) {
		acc = neg ? LONG_MIN : LONG_MAX;
		errno = ERANGE;
	} else if (neg) {
		acc = -acc;
	}

	if (endptr != NULL) *endptr = (char *)(any ? s - 1 : nptr);
	return acc;
}
