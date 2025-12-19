#ifndef RUNE_LOCALE_PARSE_H
#define RUNE_LOCALE_PARSE_H

#include <stdint.h>
#include <stddef.h>

/*
 * ============================================================================
 * Constants based on Reverse Engineering of iOS 6 LC_CTYPE
 * ============================================================================
 */
#define _CACHED_RUNES   (256)      /* Size of ASCII optimization tables */
#define _RUNE_MAGIC_A   "RuneMagA" /* Magic signature */

/*
 * ============================================================================
 * Data Structures
 * ============================================================================
 */

/* 
 * Represents a single range of Unicode characters with shared properties.
 * Corresponds to the 12-byte struct in the file at 0x0C3C.
 */
typedef struct {
    uint32_t min;           /* First unicode code point in range (Calculated) */
    uint32_t max;           /* Last unicode code point in range (From file) */
    uint32_t map;           /* Type mask or offset into variable data */
    uint32_t *types;        /* Pointer to specific types if map is an offset (NULL if uniform) */
} RuneEntry;

/*
 * Represents a case mapping (Lower <-> Upper).
 * Corresponds to the 12-byte struct in the file at 0x247C and 0x3E0C.
 */
typedef struct {
    uint32_t from;          /* Source code point */
    uint32_t to;            /* Destination code point */
} RuneCasePair;

/*
 * The main container for Locale Character Type data.
 * This structure sits in memory after parsing the binary file.
 */
typedef struct {
    /* 
     * ASCII Lookup Tables (0x00 - 0xFF)
     * Indexed by (c + 1). Index 0 is EOF (-1).
     */
    uint32_t    __s_ctype[_CACHED_RUNES + 2];   /* Character type masks */
    int32_t     __s_tolower[_CACHED_RUNES + 2]; /* To Lower mapping */
    int32_t     __s_toupper[_CACHED_RUNES + 2]; /* To Upper mapping */

    /* 
     * Unicode Range Table 
     * Used for characters > 0xFF.
     */
    int         runetype_count;     /* Number of ranges */
    RuneEntry   *runetype_ext;      /* Array of range entries */

    /*
     * Case Mappings for Unicode
     */
    int             maplower_count;
    RuneCasePair    *maplower_ext;  /* Array of Upper -> Lower mappings */

    int             mapupper_count;
    RuneCasePair    *mapupper_ext;  /* Array of Lower -> Upper mappings */

    /* 
     * Raw Variable Data Block 
     * Holds individual masks for complex ranges.
     */
    void        *variable_data;
    size_t      variable_data_len;

} RuneLocale;

/*
 * ============================================================================
 * API Functions
 * ============================================================================
 */

/*
 * Loads and parses a binary LC_CTYPE file.
 * Returns NULL on failure.
 */
RuneLocale *rune_locale_load(const char *path);

/*
 * Frees memory associated with the locale.
 */
void rune_locale_free(RuneLocale *rl);

/*
 * Core Lookup Logic (to be used by iswalpha, iswdigit, etc.)
 */
uint32_t rl_get_ctype(RuneLocale *rl, int32_t c);

/*
 * Core Case Conversion Logic (to be used by towlower, towupper)
 */
int32_t rl_toupper(RuneLocale *rl, int32_t c);
int32_t rl_tolower(RuneLocale *rl, int32_t c);

#endif // RUNE_LOCALE_PARSE_H
