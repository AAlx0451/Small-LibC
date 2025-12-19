#include "runelocaleparse.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h> /* For ntohl() - Network to Host Long */

/*
 * ============================================================================
 * Hardcoded Offsets for iOS 6 LC_CTYPE Format
 * Based on Reverse Engineering Memo
 * ============================================================================
 */
#define OFF_CTYPE       0x0034
#define OFF_TOLOWER     0x043C
#define OFF_TOUPPER     0x083C
#define OFF_RANGES      0x0C3C
#define OFF_MAPLOWER    0x247C
#define OFF_MAPUPPER    0x3E0C
#define OFF_VARDATA     0x581C

/* On-disk structure for a range entry (Big-Endian) */
typedef struct {
    uint32_t range_end;
    uint32_t ctype_mask;
    uint32_t map_offset;
} __attribute__((packed)) FileRuneEntry;

/* On-disk structure for a case map entry (Big-Endian) */
typedef struct {
    uint32_t from;
    uint32_t to;
    uint32_t flags; // Reserved
} __attribute__((packed)) FileCaseMap;

/*
 * Helper: Reads a block of 32-bit integers and converts them to Host Endian.
 */
static int read_and_convert_u32(FILE *f, void *dest, size_t count) {
    uint32_t *buf = (uint32_t *)dest;
    if (fread(buf, sizeof(uint32_t), count, f) != count) {
        return 0; // Error
    }
    for (size_t i = 0; i < count; i++) {
        buf[i] = ntohl(buf[i]);
    }
    return 1; // Success
}

RuneLocale *rune_locale_load(const char *path) {
    FILE *f = fopen(path, "rb");
    if (!f) return NULL;

    RuneLocale *rl = calloc(1, sizeof(RuneLocale));
    if (!rl) { fclose(f); return NULL; }

    /* 1. Verify Magic Header */
    char magic[8];
    if (fread(magic, 1, 8, f) != 8 || strncmp(magic, _RUNE_MAGIC_A, 8) != 0) {
        fprintf(stderr, "[RuneParser] Invalid Magic.\n");
        free(rl); fclose(f); return NULL;
    }

    /* 2. Read ASCII Tables (_ctype_, _tolower_, _toupper_) */
    /* Each table has 258 entries (EOF + 0..255) */
    size_t tbl_size = _CACHED_RUNES + 2;

    fseek(f, OFF_CTYPE, SEEK_SET);
    read_and_convert_u32(f, rl->__s_ctype, tbl_size);

    fseek(f, OFF_TOLOWER, SEEK_SET);
    read_and_convert_u32(f, rl->__s_tolower, tbl_size);

    fseek(f, OFF_TOUPPER, SEEK_SET);
    read_and_convert_u32(f, rl->__s_toupper, tbl_size);

    /* 3. Load Variable Data Block first (needed for resolving offsets) */
    fseek(f, 0, SEEK_END);
    long file_size = ftell(f);
    long vardata_size = file_size - OFF_VARDATA;
    
    if (vardata_size > 0) {
        rl->variable_data = malloc(vardata_size);
        rl->variable_data_len = vardata_size;
        fseek(f, OFF_VARDATA, SEEK_SET);
        /* 
         * Note: Variable data is essentially an array of uint32_t masks.
         * We need to convert endianness for the whole block for easier access later.
         */
        read_and_convert_u32(f, rl->variable_data, vardata_size / 4);
    }

    /* 4. Parse Unicode Ranges */
    /* Calculate count based on offsets: (NextSection - ThisSection) / Size */
    int num_ranges = (OFF_MAPLOWER - OFF_RANGES) / sizeof(FileRuneEntry);
    rl->runetype_count = num_ranges;
    rl->runetype_ext = calloc(num_ranges, sizeof(RuneEntry));

    fseek(f, OFF_RANGES, SEEK_SET);
    
    int32_t prev_end = -1; // To calculate start of range

    for (int i = 0; i < num_ranges; i++) {
        FileRuneEntry fe;
        fread(&fe, sizeof(FileRuneEntry), 1, f);
        
        uint32_t r_end = ntohl(fe.range_end);
        uint32_t r_mask = ntohl(fe.ctype_mask);
        uint32_t r_off = ntohl(fe.map_offset);

        rl->runetype_ext[i].min = (uint32_t)(prev_end + 1);
        rl->runetype_ext[i].max = r_end;
        
        if (r_off == 0) {
            /* Uniform range: all chars have 'r_mask' */
            rl->runetype_ext[i].map = r_mask;
            rl->runetype_ext[i].types = NULL; 
        } else {
            /* 
             * Complex range: 'r_off' points into variable_data.
             * According to docs, r_off is relative to start of variable_data.
             */
            rl->runetype_ext[i].map = r_off; 
            
            // Safety check
            if (r_off < (uint32_t)vardata_size) {
                rl->runetype_ext[i].types = (uint32_t *)((char *)rl->variable_data + r_off);
            }
        }
        prev_end = (int32_t)r_end;
    }

    /* 5. Parse Case Maps (Lower & Upper) */
    
    // Map Lower
    int num_lower = (OFF_MAPUPPER - OFF_MAPLOWER) / sizeof(FileCaseMap);
    rl->maplower_count = num_lower;
    rl->maplower_ext = calloc(num_lower, sizeof(RuneCasePair));
    
    fseek(f, OFF_MAPLOWER, SEEK_SET);
    for (int i = 0; i < num_lower; i++) {
        FileCaseMap fm;
        fread(&fm, sizeof(FileCaseMap), 1, f);
        rl->maplower_ext[i].from = ntohl(fm.from);
        rl->maplower_ext[i].to   = ntohl(fm.to);
    }

    // Map Upper
    int num_upper = (OFF_VARDATA - OFF_MAPUPPER) / sizeof(FileCaseMap);
    rl->mapupper_count = num_upper;
    rl->mapupper_ext = calloc(num_upper, sizeof(RuneCasePair));

    fseek(f, OFF_MAPUPPER, SEEK_SET);
    for (int i = 0; i < num_upper; i++) {
        FileCaseMap fm;
        fread(&fm, sizeof(FileCaseMap), 1, f);
        rl->mapupper_ext[i].from = ntohl(fm.from);
        rl->mapupper_ext[i].to   = ntohl(fm.to);
    }

    fclose(f);
    return rl;
}

void rune_locale_free(RuneLocale *rl) {
    if (!rl) return;
    if (rl->runetype_ext) free(rl->runetype_ext);
    if (rl->maplower_ext) free(rl->maplower_ext);
    if (rl->mapupper_ext) free(rl->mapupper_ext);
    if (rl->variable_data) free(rl->variable_data);
    free(rl);
}

/*
 * ============================================================================
 * Logic Implementation
 * ============================================================================
 */

uint32_t rl_get_ctype(RuneLocale *rl, int32_t c) {
    if (c < 0 || c == EOF) return 0; // EOF usually has index 0 in internal array

    /* Optimization: ASCII direct lookup */
    if (c < _CACHED_RUNES) {
        return rl->__s_ctype[c + 1];
    }

    /* Binary Search for Unicode Ranges */
    RuneEntry *base = rl->runetype_ext;
    int lim = rl->runetype_count;
    RuneEntry *p;

    while (lim != 0) {
        p = base + (lim >> 1); /* integer division by 2 */
        if (c <= p->max) {
            if (c >= p->min) {
                /* Found the range containing 'c' */
                
                if (p->types) {
                    /* Complex range: fetch individual mask from variable data */
                    return p->types[c - p->min];
                } else {
                    /* Uniform range: return the common mask */
                    return p->map;
                }
            }
            /* c is smaller than this range, search left half */
            lim >>= 1;
        } else {
            /* c is larger, search right half */
            base = p + 1;
            lim--;
            lim >>= 1;
        }
    }
    
    return 0; // Not found (default)
}

static int32_t _rl_map_case(RuneCasePair *map, int count, int32_t c) {
    /* Binary Search */
    RuneCasePair *base = map;
    int lim = count;
    RuneCasePair *p;

    while (lim != 0) {
        p = base + (lim >> 1);
        if (c == p->from) {
            return p->to; /* Match found */
        }
        if (c < p->from) {
            lim >>= 1;
        } else {
            base = p + 1;
            lim--;
            lim >>= 1;
        }
    }
    return c; /* No mapping found, return original */
}

int32_t rl_tolower(RuneLocale *rl, int32_t c) {
    if (c < 0 || c == EOF) return c;
    
    /* ASCII Optimization */
    if (c < _CACHED_RUNES) {
        return rl->__s_tolower[c + 1];
    }
    
    /* Unicode Lookup */
    return _rl_map_case(rl->maplower_ext, rl->maplower_count, c);
}

int32_t rl_toupper(RuneLocale *rl, int32_t c) {
    if (c < 0 || c == EOF) return c;

    /* ASCII Optimization */
    if (c < _CACHED_RUNES) {
        return rl->__s_toupper[c + 1];
    }

    /* Unicode Lookup */
    return _rl_map_case(rl->mapupper_ext, rl->mapupper_count, c);
}
