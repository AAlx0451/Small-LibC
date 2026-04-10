#include <locale.h>

int rl_ispunct(RuneLocale *rl, int c) { return rl_istype(rl, c, RL_CTYPE_P); }
