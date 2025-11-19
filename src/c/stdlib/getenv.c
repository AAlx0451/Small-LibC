#include <stdlib.h>
#include <string.h>

extern char **environ;

char *getenv(const char *name) {
	size_t len;
	char **p;

	if (name == NULL || environ == NULL) {
		return NULL;
	}

	len = strlen(name);
	for (p = environ; *p != NULL; p++) {
		if (strncmp(*p, name, len) == 0 && (*p)[len] == '=') {
			return *p + len + 1;
		}
	}

	return NULL;
}
