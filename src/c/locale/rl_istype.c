#include <locale.h>

int rl_istype(RuneLocale *rl, int c, unsigned long f) { return !!(rl_runetype(rl, c) & f); }
