#include <locale.h>

int rl_islower(RuneLocale *rl, int c) { return rl_istype(rl, c, RL_CTYPE_L); }
