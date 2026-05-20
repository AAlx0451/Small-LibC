#include <stdio.h>
#include <wchar.h>

wint_t getwchar(void) { return getwc(stdin); }
