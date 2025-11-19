#include <stdlib.h>
#include <unistd.h>

void abort(void) {
	const char msg[] = "Aborted\n";
	write(2, msg, 8);
	_exit(134);
}
