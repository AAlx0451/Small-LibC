#include <unistd.h>
#include <stdio.h>

int putchar(int c) {
  char character = (char)c;
  if (write(1, &character, 1) != 1) {
    return EOF;
  }
  return (int)(unsigned char)character;
}
