/*
 * rlparse.c
 *
 * iOS 6 LC_CTYPE Binary Parser.
 *
 * TECHNICAL DESCRIPTION:
 * This parser handles the specific binary layout of iOS 6 locale files.
 * Unlike standard BSD implementations, this format uses a "Dynamic Header"
 * located at offset 0x0C34 to store table counts, and strictly aligns
 * all data structures to 16 bytes.
 *
 * KEY OFFSETS:
 * - 0x0034: ASCII CTYPE Table
 * - 0x0C34: Hidden Header (Contains counts for Ranges, MapLower, MapUpper)
 * - 0x0C4C: Start of Unicode Ranges
 * - 0x????: Start of Maps (Calculated dynamically based on counts)
 */

#include "runelocaleparse.h"
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ================= OFFSETS ================= */

/* Standard Header Offsets (Fixed in this format version) */
#define OFF_CTYPE 0x0034
#define OFF_TOLOWER 0x0434
#define OFF_TOUPPER 0x0834

/*
 * The "Hidden" Header.
 * Located immediately after the 3rd ASCII table (0x0834 + 0x400 = 0x0C34).
 * Stores the number of entries for the variable-length tables.
 */
#define OFF_COUNTS 0x0C34

/* Base offset where the first variable table (Ranges) always begins */
#define OFF_RANGES_BASE 0x0C4C

/* ================= ON-DISK STRUCTURES ================= */

/*
 * FileRuneEntry16
 * Represents a range in the Ranges table.
 * Size: 16 bytes (Packed, Big-Endian)
 */
typedef struct {
    uint32_t min;   /* Start of range */
    uint32_t max;   /* End of range */
    uint32_t map;   /* Default Type Mask */
    uint32_t flags; /* Flags: 0x01000000 implies Variable Data exists */
} __attribute__((packed)) FileRuneEntry16;

/*
 * FileCaseMap16
 * Represents a range in MapLower/MapUpper tables.
 * Size: 16 bytes (Packed, Big-Endian) - Note the 4-byte padding!
 */
typedef struct {
    uint32_t min; /* Start of range */
    uint32_t max; /* End of range */
    int32_t map;  /* Base value for conversion */
    uint32_t pad; /* Padding/Junk (Must skip this!) */
} __attribute__((packed)) FileCaseMap16;

/* ================= HELPER FUNCTIONS ================= */

/* Reads 'count' 32-bit integers, converting from Big-Endian to Host */
static int read_u32_array(FILE *f, void *dest, size_t count) {
    uint32_t *buf = (uint32_t *)dest;
    if(fread(buf, sizeof(uint32_t), count, f) != count)
        return 0;
    for(size_t i = 0; i < count; i++)
        buf[i] = ntohl(buf[i]);
    return 1;
}

/* Reads a single 32-bit integer from a specific offset */
static uint32_t read_u32(FILE *f, long offset) {
    uint32_t val;
    fseek(f, offset, SEEK_SET);
    fread(&val, sizeof(uint32_t), 1, f);
    return ntohl(val);
}

/* ================= MAIN PARSER ================= */

RuneLocale *rune_locale_load(const char *path) {
    FILE *f = fopen(path, "rb");
    if(!f)
        return NULL;

    RuneLocale *rl = calloc(1, sizeof(RuneLocale));
    if(!rl) {
        fclose(f);
        return NULL;
    }

    /*
     * 1. Load ASCII Tables
     * The file stores exactly 256 entries (0x00-0xFF).
     * We map them to indices 1-256 in our array, reserving index 0 for EOF.
     */

    // CTYPE
    fseek(f, OFF_CTYPE, SEEK_SET);
    rl->__s_ctype[0] = 0;
    read_u32_array(f, &rl->__s_ctype[1], _CACHED_RUNES);

    // TOLOWER
    fseek(f, OFF_TOLOWER, SEEK_SET);
    rl->__s_tolower[0] = EOF;
    read_u32_array(f, &rl->__s_tolower[1], _CACHED_RUNES);

    // TOUPPER
    fseek(f, OFF_TOUPPER, SEEK_SET);
    rl->__s_toupper[0] = EOF;
    read_u32_array(f, &rl->__s_toupper[1], _CACHED_RUNES);

    /*
     * 2. Read Dynamic Counts
     * The header at 0x0C34 stores counts separated by 4-byte padding.
     * Stride appears to be 8 bytes.
     */
    uint32_t cnt_ranges = read_u32(f, OFF_COUNTS);
    uint32_t cnt_lower = read_u32(f, OFF_COUNTS + 8);
    uint32_t cnt_upper = read_u32(f, OFF_COUNTS + 16);

    /*
     * 3. Calculate Table Offsets
     * All tables are 16-byte aligned structures.
     */
    long off_ranges = OFF_RANGES_BASE;
    long off_maplower = off_ranges + (cnt_ranges * 16);
    long off_mapupper = off_maplower + (cnt_lower * 16);
    long off_vardata = off_mapupper + (cnt_upper * 16);

    /* Check file integrity */
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    if(off_vardata > fsize) {
        free(rl);
        fclose(f);
        return NULL;
    }

    /*
     * 4. Load Case Maps
     */

    // Map Lower
    rl->maplower_count = cnt_lower;
    rl->maplower_ext = calloc(cnt_lower, sizeof(RuneCasePair));
    fseek(f, off_maplower, SEEK_SET);
    for(int i = 0; i < cnt_lower; i++) {
        FileCaseMap16 fm;
        fread(&fm, sizeof(FileCaseMap16), 1, f);
        rl->maplower_ext[i].min = ntohl(fm.min);
        rl->maplower_ext[i].max = ntohl(fm.max);
        rl->maplower_ext[i].map = ntohl(fm.map);
    }

    // Map Upper
    rl->mapupper_count = cnt_upper;
    rl->mapupper_ext = calloc(cnt_upper, sizeof(RuneCasePair));
    fseek(f, off_mapupper, SEEK_SET);
    for(int i = 0; i < cnt_upper; i++) {
        FileCaseMap16 fm;
        fread(&fm, sizeof(FileCaseMap16), 1, f);
        rl->mapupper_ext[i].min = ntohl(fm.min);
        rl->mapupper_ext[i].max = ntohl(fm.max);
        rl->mapupper_ext[i].map = ntohl(fm.map);
    }

    /*
     * 5. Load Variable Data
     * This blob contains arrays of types for complex ranges.
     */
    long vardata_len = fsize - off_vardata;
    if(vardata_len > 0) {
        rl->variable_data = malloc(vardata_len);
        rl->variable_data_len = vardata_len;
        fseek(f, off_vardata, SEEK_SET);
        read_u32_array(f, rl->variable_data, vardata_len / 4);
    }

    /*
     * 6. Load Ranges & Link Data
     */
    rl->runetype_count = cnt_ranges;
    rl->runetype_ext = calloc(cnt_ranges, sizeof(RuneEntry));
    fseek(f, off_ranges, SEEK_SET);

    uint32_t *vdata_ptr = (uint32_t *)rl->variable_data;
    uint32_t *vdata_end = (uint32_t *)((char *)rl->variable_data + vardata_len);

    for(int i = 0; i < cnt_ranges; i++) {
        FileRuneEntry16 fe;
        fread(&fe, sizeof(FileRuneEntry16), 1, f);

        rl->runetype_ext[i].min = ntohl(fe.min);
        rl->runetype_ext[i].max = ntohl(fe.max);
        uint32_t map = ntohl(fe.map);
        uint32_t flag = ntohl(fe.flags);

        rl->runetype_ext[i].map = map;

        /*
         * Linkage Logic:
         * If 'flag' is set (non-zero), it means this range has specific types
         * stored in Variable Data. We consume the next N integers from vdata,
         * where N is the size of the range.
         */
        if(flag != 0 && rl->variable_data) {
            uint32_t count = rl->runetype_ext[i].max - rl->runetype_ext[i].min + 1;

            if(vdata_ptr + count <= vdata_end) {
                rl->runetype_ext[i].types = vdata_ptr;
                vdata_ptr += count;
            } else {
                rl->runetype_ext[i].types = NULL;
            }
        } else {
            rl->runetype_ext[i].types = NULL;
        }
    }

    fclose(f);
    return rl;
}

void rune_locale_free(RuneLocale *rl) {
    if(!rl)
        return;
    if(rl->runetype_ext)
        free(rl->runetype_ext);
    if(rl->maplower_ext)
        free(rl->maplower_ext);
    if(rl->mapupper_ext)
        free(rl->mapupper_ext);
    if(rl->variable_data)
        free(rl->variable_data);
    free(rl);
}

/* ================= LOGIC API ================= */

uint32_t rl_get_ctype(RuneLocale *rl, int32_t c) {
    if(c < 0 || c == EOF)
        return 0;

    /* Fast path for ASCII */
    if(c < _CACHED_RUNES)
        return rl->__s_ctype[c + 1];

    /* Binary search for Unicode ranges */
    RuneEntry *base = rl->runetype_ext;
    int lim = rl->runetype_count;
    RuneEntry *p;

    while(lim != 0) {
        p = base + (lim >> 1);
        if(c <= p->max) {
            if(c >= p->min) {
                return p->types ? p->types[c - p->min] : p->map;
            }
            lim >>= 1;
        } else {
            base = p + 1;
            lim--;
            lim >>= 1;
        }
    }
    return 0;
}

static int32_t _rl_search_map(RuneCasePair *map, int count, int32_t c) {
    RuneCasePair *base = map;
    int lim = count;
    RuneCasePair *p;

    while(lim != 0) {
        p = base + (lim >> 1);
        if(c <= p->max) {
            if(c >= p->min) {
                return p->map + (c - p->min);
            }
            lim >>= 1;
        } else {
            base = p + 1;
            lim--;
            lim >>= 1;
        }
    }
    return c;
}

int32_t rl_tolower(RuneLocale *rl, int32_t c) {
    if(c < 0 || c == EOF)
        return c;
    if(c < _CACHED_RUNES)
        return rl->__s_tolower[c + 1];
    return _rl_search_map(rl->maplower_ext, rl->maplower_count, c);
}

int32_t rl_toupper(RuneLocale *rl, int32_t c) {
    if(c < 0 || c == EOF)
        return c;
    if(c < _CACHED_RUNES)
        return rl->__s_toupper[c + 1];
    return _rl_search_map(rl->mapupper_ext, rl->mapupper_count, c);
}
