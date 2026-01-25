#include <sys/select.h>

int pause(void) {
	return select(0, NULL, NULL, NULL, NULL);
}
