/*
 * runelocaleparse.h
 *
 * iOS 6 LC_CTYPE Binary Parser
 *
 * This header defines the In-Memory representation of the RuneLocale structure.
 * It is designed to handle the proprietary binary format found in /usr/share/locale
 * on legacy OSX/iOS systems.
 *
 * REVERSE ENGINEERING SUMMARY:
 * - Format: Big-Endian.
 * - Alignment: Strict 16-byte alignment for all Unicode tables
 * - ASCII Tables: 256 entries (0x00-0xFF), missing the standard EOF entry at index 0.
 */

#ifndef RUNE_LOCALE_PARSE_H
#define RUNE_LOCALE_PARSE_H

#include <stdint.h>
#include <stdio.h> /* for EOF */

/* Magic signature found at the start of the file */
#define _RUNE_MAGIC_A   "RuneMagA"

/* Size of the ASCII cache tables (0-255) */
#define _CACHED_RUNES   (256)

/**
 * RuneEntry (In-Memory)
 * Represents a contiguous range of Unicode characters with specific properties.
 * 
 * @min:   First character code in the range.
 * @max:   Last character code in the range.
 * @map:   Base property mask (CTYPE) for this range.
 * @types: Pointer to an array of property masks in variable_data. 
 *         Used when characters in the range have differing properties.
 *         If NULL, all characters in range use 'map'.
 */
typedef struct {
    uint32_t min;
    uint32_t max;
    uint32_t map;
    uint32_t *types;
} RuneEntry;

/**
 * RuneCasePair (In-Memory)
 * Represents a case mapping range (tolower/toupper).
 * 
 * @min: First character code.
 * @max: Last character code.
 * @map: Base value for conversion.
 *       Algorithm: converted_char = map + (original_char - min)
 */
typedef struct {
    uint32_t min;
    uint32_t max;
    int32_t  map; 
} RuneCasePair;

/**
 * RuneLocale (In-Memory)
 * The main container for locale data, mimicking the internal libc structure.
 */
typedef struct {
    /* 
     * ASCII Look-up Tables (Cache).
     * The internal libc logic uses index 0 for EOF (-1).
     * Indices 1..256 correspond to char codes 0x00..0xFF.
     */
    uint32_t    __s_ctype[_CACHED_RUNES + 1];
    int32_t     __s_tolower[_CACHED_RUNES + 1];
    int32_t     __s_toupper[_CACHED_RUNES + 1];

    /* Unicode Character Properties Ranges */
    int         runetype_count;
    RuneEntry   *runetype_ext;

    /* Unicode To-Lower Mapping Ranges */
    int         maplower_count;
    RuneCasePair *maplower_ext;

    /* Unicode To-Upper Mapping Ranges */
    int         mapupper_count;
    RuneCasePair *mapupper_ext;

    /* Storage blob for variable-length type arrays */
    void        *variable_data;
    size_t      variable_data_len;
} RuneLocale;

/**
 * rune_locale_load
 * Parses the LC_CTYPE binary file using dynamic header analysis.
 * 
 * @path: Filesystem path to the locale file.
 * @return: Pointer to allocated RuneLocale, or NULL on failure.
 */
RuneLocale *rune_locale_load(const char *path);

/**
 * rune_locale_free
 * Frees all memory associated with the locale.
 */
void rune_locale_free(RuneLocale *rl);

/* 
 * API Functions 
 * These provide the standard functionality of <ctype.h> / <wctype.h>.
 */
uint32_t rl_get_ctype(RuneLocale *rl, int32_t c);
int32_t rl_toupper(RuneLocale *rl, int32_t c);
int32_t rl_tolower(RuneLocale *rl, int32_t c);

#endif // RUNE_LOCALE_PARSE_H
