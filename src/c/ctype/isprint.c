#include <ctype.h>
#include <locale.h>

int isprint(int c)
{
    return (c > 255 || c < 0) ? 0 : (!!(_CurrentRuneLocale->runetype[c] & RL_CTYPE_R));
}
