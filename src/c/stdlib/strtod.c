#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <float.h>

double strtod(const char *nptr, char **endptr) {
	const char *s = nptr;
	double acc = 0.0;
	int sign = 1;
	int exp_sign = 1;
	int exp = 0;
	double power = 1.0;
	int frac_part = 0;

	while (isspace(*s)) s++;

	if (*s == '-') {
		sign = -1;
		s++;
	} else if (*s == '+') {
		s++;
	}

	if (endptr != NULL) *endptr = (char *)nptr;

	while (isdigit(*s)) {
		acc = acc * 10.0 + (*s - '0');
		s++;
		if (endptr != NULL) *endptr = (char *)s;
	}

	if (*s == '.') {
		s++;
		while (isdigit(*s)) {
			acc = acc * 10.0 + (*s - '0');
			power *= 10.0;
			s++;
			if (endptr != NULL) *endptr = (char *)s;
		}
	}

	if (*s == 'e' || *s == 'E') {
		s++;
		if (*s == '-') {
			exp_sign = -1;
			s++;
		} else if (*s == '+') {
			s++;
		}
		while (isdigit(*s)) {
			exp = exp * 10 + (*s - '0');
			s++;
			if (endptr != NULL) *endptr = (char *)s;
		}
	}

	acc = sign * acc / power;

	while (exp > 0) {
		if (exp_sign == 1) acc *= 10.0;
		else acc /= 10.0;
		exp--;
	}

	return acc;
}
