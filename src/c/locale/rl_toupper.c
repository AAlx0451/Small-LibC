#include <locale.h>

int rl_toupper(RuneLocale *rl, int c) {
    if(c < 0)
        return c;
    if(c < 256)
        return rl->mapupper[c];

    size_t lim;
    RuneEntry *base = rl->mapupper_ext.ranges;
    for(lim = rl->mapupper_ext.nranges; lim != 0; lim >>= 1) {
        RuneEntry *re = base + (lim >> 1);
        if(re->min <= (uint32_t)c && (uint32_t)c <= re->max) {
            return re->map + c - re->min;
        } else if((uint32_t)c > re->max) {
            base = re + 1;
            lim--;
        }
    }
    return c;
}
