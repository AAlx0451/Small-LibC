#include <locale.h>

uint32_t rl_runetype(RuneLocale *rl, int c) {
    if(c < 0)
        return 0;
    if(c < 256)
        return rl->runetype[c];

    size_t lim;
    RuneEntry *base = rl->runetype_ext.ranges;
    for(lim = rl->runetype_ext.nranges; lim != 0; lim >>= 1) {
        RuneEntry *re = base + (lim >> 1);
        if(re->min <= (uint32_t)c && (uint32_t)c <= re->max) {
            return re->types ? re->types[c - re->min] : (uint32_t)re->map;
        } else if((uint32_t)c > re->max) {
            base = re + 1;
            lim--;
        }
    }
    return 0;
}
