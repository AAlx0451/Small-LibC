#include <locale.h>

int rl_isgraph(RuneLocale *rl, int c) { return rl_istype(rl, c, RL_CTYPE_G); }
