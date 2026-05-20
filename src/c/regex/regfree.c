#include "regex_internal.h"
#include <stdlib.h>

void regfree(regex_t *preg)
{
    if (!preg || !preg->re_comp)
        return;
    CompData *cd = (CompData *)preg->re_comp;
    if (cd->nodes)
        free(cd->nodes);
    if (cd->pat)
        free(cd->pat);
    free(cd);
    preg->re_comp = NULL;
}
