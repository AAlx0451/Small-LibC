#include <sys/select.h>
#include <unistd.h>

int pause(void) {
    return select(0, NULL, NULL, NULL, NULL);
}
