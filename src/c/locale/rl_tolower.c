#include <locale.h>

int rl_tolower(RuneLocale *rl, int c) {
    if(c < 0)
        return c;
    if(c < 256)
        return getmap(lower, rl, c);

    size_t lim;
    uint32_t uc = (uint32_t)c;
    RuneEntry *base = rl->maplower_ext.ranges;
    for(lim = (size_t)rl->maplower_ext.nranges; lim != 0; lim >>= 1) {
        RuneEntry *re = base + (lim >> 1);
        if(re->min <= uc && uc <= re->max) {
            return re->map + c - (int)re->min;
        } else if(uc > re->max) {
            base = re + 1;
            lim--;
        }
    }
    return c;
}
