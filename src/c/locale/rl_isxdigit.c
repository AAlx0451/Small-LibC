#include <locale.h>

int rl_isxdigit(RuneLocale *rl, int c) { return rl_istype(rl, c, RL_CTYPE_X); }
