#include <locale.h>

int rl_isalpha(RuneLocale *rl, int c) { return rl_istype(rl, c, RL_CTYPE_A); }
