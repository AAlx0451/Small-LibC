#include "runelocale.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>

RuneLocale *rl_load_mem(const void *data, size_t size) {
    if (size < 3164) return NULL;
    
    const uint8_t *p = (const uint8_t *)data;
    if (memcmp(p, "RuneMagA", 8) != 0) return NULL;
    
    RuneLocale *rl = calloc(1, sizeof(RuneLocale));
    if (!rl) return NULL;
    
    for (int i = 0; i < 256; i++) {
        uint32_t val;
        memcpy(&val, p + 52 + i * 4, 4);
        rl->runetype[i] = ntohl(val);
        
        memcpy(&val, p + 1076 + i * 4, 4);
        rl->maplower[i] = ntohl(val);
        
        memcpy(&val, p + 2100 + i * 4, 4);
        rl->mapupper[i] = ntohl(val);
    }
    
    uint32_t val;
    memcpy(&val, p + 3124, 4); rl->runetype_ext.nranges = ntohl(val);
    memcpy(&val, p + 3132, 4); rl->maplower_ext.nranges = ntohl(val);
    memcpy(&val, p + 3140, 4); rl->mapupper_ext.nranges = ntohl(val);
    memcpy(&val, p + 3152, 4); uint32_t variable_len = ntohl(val);
    memcpy(&val, p + 3156, 4); int ncharclasses = ntohl(val);

    const uint8_t *ptr = p + 3164;
    const uint8_t *end = p + size;

    if (rl->runetype_ext.nranges > 0) {
        size_t req = (size_t)rl->runetype_ext.nranges * 16;
        if ((size_t)(end - ptr) < req) goto fail;
        rl->runetype_ext.ranges = calloc(rl->runetype_ext.nranges, sizeof(RuneEntry));
        if (!rl->runetype_ext.ranges) goto fail;
        for (int i = 0; i < rl->runetype_ext.nranges; i++) {
            uint32_t minv, maxv, mapv;
            memcpy(&minv, ptr, 4);
            memcpy(&maxv, ptr + 4, 4);
            memcpy(&mapv, ptr + 8, 4);
            rl->runetype_ext.ranges[i].min = ntohl(minv);
            rl->runetype_ext.ranges[i].max = ntohl(maxv);
            rl->runetype_ext.ranges[i].map = (int32_t)ntohl(mapv);
            ptr += 16;
        }
    } else if (rl->runetype_ext.nranges < 0) goto fail;

    if (rl->maplower_ext.nranges > 0) {
        size_t req = (size_t)rl->maplower_ext.nranges * 16;
        if ((size_t)(end - ptr) < req) goto fail;
        rl->maplower_ext.ranges = calloc(rl->maplower_ext.nranges, sizeof(RuneEntry));
        if (!rl->maplower_ext.ranges) goto fail;
        for (int i = 0; i < rl->maplower_ext.nranges; i++) {
            uint32_t minv, maxv, mapv;
            memcpy(&minv, ptr, 4);
            memcpy(&maxv, ptr + 4, 4);
            memcpy(&mapv, ptr + 8, 4);
            rl->maplower_ext.ranges[i].min = ntohl(minv);
            rl->maplower_ext.ranges[i].max = ntohl(maxv);
            rl->maplower_ext.ranges[i].map = (int32_t)ntohl(mapv);
            ptr += 16;
        }
    } else if (rl->maplower_ext.nranges < 0) goto fail;

    if (rl->mapupper_ext.nranges > 0) {
        size_t req = (size_t)rl->mapupper_ext.nranges * 16;
        if ((size_t)(end - ptr) < req) goto fail;
        rl->mapupper_ext.ranges = calloc(rl->mapupper_ext.nranges, sizeof(RuneEntry));
        if (!rl->mapupper_ext.ranges) goto fail;
        for (int i = 0; i < rl->mapupper_ext.nranges; i++) {
            uint32_t minv, maxv, mapv;
            memcpy(&minv, ptr, 4);
            memcpy(&maxv, ptr + 4, 4);
            memcpy(&mapv, ptr + 8, 4);
            rl->mapupper_ext.ranges[i].min = ntohl(minv);
            rl->mapupper_ext.ranges[i].max = ntohl(maxv);
            rl->mapupper_ext.ranges[i].map = (int32_t)ntohl(mapv);
            ptr += 16;
        }
    } else if (rl->mapupper_ext.nranges < 0) goto fail;

    for (int i = 0; i < rl->runetype_ext.nranges; i++) {
        if (rl->runetype_ext.ranges[i].map == 0) {
            uint32_t minv = rl->runetype_ext.ranges[i].min;
            uint32_t maxv = rl->runetype_ext.ranges[i].max;
            if (maxv < minv) goto fail;
            uint32_t len = maxv - minv + 1;
            
            if ((size_t)(end - ptr) < len * 4) goto fail;
            
            rl->runetype_ext.ranges[i].types = calloc(len, 4);
            if (!rl->runetype_ext.ranges[i].types) goto fail;
            
            for (uint32_t j = 0; j < len; j++) {
                uint32_t v;
                memcpy(&v, ptr + j * 4, 4);
                rl->runetype_ext.ranges[i].types[j] = ntohl(v);
            }
            ptr += len * 4;
        }
    }

    if (ncharclasses > 0) {
        if ((size_t)(end - ptr) < (size_t)ncharclasses * 20) goto fail;
        ptr += ncharclasses * 20;
    } else if (ncharclasses < 0) goto fail;

    if (variable_len > 0) {
        if ((size_t)(end - ptr) < variable_len) goto fail;
    }

    return rl;

fail:
    rl_free(rl);
    return NULL;
}

RuneLocale *rl_load_file(const char *path) {
    FILE *fp = fopen(path, "rb");
    if (!fp) return NULL;
    
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    
    if (size < 3164) {
        fclose(fp);
        return NULL;
    }
    
    void *data = malloc(size);
    if (!data) {
        fclose(fp);
        return NULL;
    }
    
    if (fread(data, 1, size, fp) != (size_t)size) {
        free(data);
        fclose(fp);
        return NULL;
    }
    fclose(fp);
    
    RuneLocale *rl = rl_load_mem(data, size);
    free(data);
    return rl;
}

void rl_free(RuneLocale *rl) {
    if (!rl) return;
    if (rl->runetype_ext.ranges) {
        for (int i = 0; i < rl->runetype_ext.nranges; i++) {
            free(rl->runetype_ext.ranges[i].types);
        }
        free(rl->runetype_ext.ranges);
    }
    free(rl->maplower_ext.ranges);
    free(rl->mapupper_ext.ranges);
    free(rl);
}

uint32_t rl_runetype(RuneLocale *rl, int c) {
    if (c < 0) return 0;
    if (c < 256) return rl->runetype[c];
    
    size_t lim;
    RuneEntry *base = rl->runetype_ext.ranges;
    for (lim = rl->runetype_ext.nranges; lim != 0; lim >>= 1) {
        RuneEntry *re = base + (lim >> 1);
        if (re->min <= (uint32_t)c && (uint32_t)c <= re->max) {
            return re->types ? re->types[c - re->min] : (uint32_t)re->map;
        } else if ((uint32_t)c > re->max) {
            base = re + 1;
            lim--;
        }
    }
    return 0;
}

int rl_tolower(RuneLocale *rl, int c) {
    if (c < 0) return c;
    if (c < 256) return rl->maplower[c];
    
    size_t lim;
    RuneEntry *base = rl->maplower_ext.ranges;
    for (lim = rl->maplower_ext.nranges; lim != 0; lim >>= 1) {
        RuneEntry *re = base + (lim >> 1);
        if (re->min <= (uint32_t)c && (uint32_t)c <= re->max) {
            return re->map + c - re->min;
        } else if ((uint32_t)c > re->max) {
            base = re + 1;
            lim--;
        }
    }
    return c;
}

int rl_toupper(RuneLocale *rl, int c) {
    if (c < 0) return c;
    if (c < 256) return rl->mapupper[c];
    
    size_t lim;
    RuneEntry *base = rl->mapupper_ext.ranges;
    for (lim = rl->mapupper_ext.nranges; lim != 0; lim >>= 1) {
        RuneEntry *re = base + (lim >> 1);
        if (re->min <= (uint32_t)c && (uint32_t)c <= re->max) {
            return re->map + c - re->min;
        } else if ((uint32_t)c > re->max) {
            base = re + 1;
            lim--;
        }
    }
    return c;
}
