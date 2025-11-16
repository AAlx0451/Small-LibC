#include <string.h>
#include <unistd.h>
#include <stdio.h>

int puts(const char *str) {
	if (str == NULL) {
		str = "(null)";
	}
	int len = strlen(str);
	if (write(1, str, len) == -1) {
		return EOF;
	}
	if (write(1, "\n", 1) == -1) {
		return EOF;
	}
	return 0;
}
