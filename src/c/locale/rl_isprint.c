#include <locale.h>

int rl_isprint(RuneLocale *rl, int c) { return rl_istype(rl, c, RL_CTYPE_R); }
