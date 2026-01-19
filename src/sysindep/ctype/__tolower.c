#include <ctype.h>

unsigned char __tolower(unsigned char c) {
  if (isupper(c)) {
    c -= 'A' - 'a';
  }

  return c;
}
