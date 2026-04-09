#ifndef RUNELOCALE_H
#define RUNELOCALE_H

#include <stdint.h>
#include <stddef.h>

#define RL_CTYPE_A 0x00000100L
#define RL_CTYPE_C 0x00000200L
#define RL_CTYPE_D 0x00000400L
#define RL_CTYPE_G 0x00000800L
#define RL_CTYPE_L 0x00001000L
#define RL_CTYPE_P 0x00002000L
#define RL_CTYPE_S 0x00004000L
#define RL_CTYPE_U 0x00008000L
#define RL_CTYPE_X 0x00010000L
#define RL_CTYPE_B 0x00020000L
#define RL_CTYPE_R 0x00040000L
#define RL_CTYPE_I 0x00080000L
#define RL_CTYPE_T 0x00100000L
#define RL_CTYPE_Q 0x00200000L

typedef struct {
    uint32_t min;
    uint32_t max;
    int32_t map;
    uint32_t *types;
} RuneEntry;

typedef struct {
    int nranges;
    RuneEntry *ranges;
} RuneRange;

typedef struct {
    uint32_t runetype[256];
    uint32_t maplower[256];
    uint32_t mapupper[256];
    RuneRange runetype_ext;
    RuneRange maplower_ext;
    RuneRange mapupper_ext;
} RuneLocale;

RuneLocale *rl_load_mem(const void *data, size_t size);
RuneLocale *rl_load_file(const char *path);
void rl_free(RuneLocale *rl);

uint32_t rl_runetype(RuneLocale *rl, int c);
int rl_tolower(RuneLocale *rl, int c);
int rl_toupper(RuneLocale *rl, int c);

static inline int rl_istype(RuneLocale *rl, int c, unsigned long f) {
    return !!(rl_runetype(rl, c) & f);
}

static inline int rl_isascii(int c) { return (c & ~0x7F) == 0; }
static inline int rl_isalnum(RuneLocale *rl, int c) { return rl_istype(rl, c, RL_CTYPE_A | RL_CTYPE_D); }
static inline int rl_isalpha(RuneLocale *rl, int c) { return rl_istype(rl, c, RL_CTYPE_A); }
static inline int rl_isblank(RuneLocale *rl, int c) { return rl_istype(rl, c, RL_CTYPE_B); }
static inline int rl_iscntrl(RuneLocale *rl, int c) { return rl_istype(rl, c, RL_CTYPE_C); }
static inline int rl_isdigit(RuneLocale *rl, int c) { return rl_istype(rl, c, RL_CTYPE_D); }
static inline int rl_isgraph(RuneLocale *rl, int c) { return rl_istype(rl, c, RL_CTYPE_G); }
static inline int rl_islower(RuneLocale *rl, int c) { return rl_istype(rl, c, RL_CTYPE_L); }
static inline int rl_isprint(RuneLocale *rl, int c) { return rl_istype(rl, c, RL_CTYPE_R); }
static inline int rl_ispunct(RuneLocale *rl, int c) { return rl_istype(rl, c, RL_CTYPE_P); }
static inline int rl_isspace(RuneLocale *rl, int c) { return rl_istype(rl, c, RL_CTYPE_S); }
static inline int rl_isupper(RuneLocale *rl, int c) { return rl_istype(rl, c, RL_CTYPE_U); }
static inline int rl_isxdigit(RuneLocale *rl, int c) { return rl_istype(rl, c, RL_CTYPE_X); }

#endif
