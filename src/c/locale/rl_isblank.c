#include <locale.h>

int rl_isblank(RuneLocale *rl, int c) { return rl_istype(rl, c, RL_CTYPE_B); }
