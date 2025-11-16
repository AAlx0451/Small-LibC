#include <unistd.h>

__attribute__((noreturn)) void exit(int status) {
  // TODO: atexit()
  // TODO: fflush(NULL)
  _exit(status);
}
