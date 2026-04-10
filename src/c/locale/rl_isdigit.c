#include <locale.h>

int rl_isdigit(RuneLocale *rl, int c) { return rl_istype(rl, c, RL_CTYPE_D); }
