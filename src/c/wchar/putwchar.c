#include <stdio.h>
#include <wchar.h>

wint_t putwchar(wchar_t c) { return putwc(c, stdout); }
