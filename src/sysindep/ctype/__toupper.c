#include <ctype.h>

unsigned char __toupper(unsigned char c) {
  if (islower(c)) {
    c -= 'a' - 'A';
  }

  return c;
}
