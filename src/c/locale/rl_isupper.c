#include <locale.h>

int rl_isupper(RuneLocale *rl, int c) { return rl_istype(rl, c, RL_CTYPE_U); }
