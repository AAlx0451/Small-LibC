#include <stdio.h>
#include <wchar.h>

wint_t getwc(FILE *stream) { return fgetwc(stream); }
