#include <locale.h>

int rl_isalnum(RuneLocale *rl, int c) { return rl_istype(rl, c, RL_CTYPE_A | RL_CTYPE_D); }
