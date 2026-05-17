#include <locale.h>

uint32_t rl_runetype(RuneLocale *rl, int c) {
    if(c < 0)
        return 0;
    if(c < 256)
        return rl->runetype[c];

    size_t lim;
    uint32_t uc = (uint32_t)c;

    RuneEntry *base = rl->runetype_ext.ranges;
    for(lim = (size_t)rl->runetype_ext.nranges; lim != 0; lim >>= 1) {
        RuneEntry *re = base + (lim >> 1);
        if(re->min <= uc && uc <= re->max) {
            return re->types ? re->types[uc - re->min] : (uint32_t)re->map;
        } else if(uc > re->max) {
            base = re + 1;
            lim--;
        }
    }
    return 0;
}
