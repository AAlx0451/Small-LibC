#ifndef LOCALE_H
#define LOCALE_H

#include <stddef.h>
#include <stdint.h>
#include <limits.h>
#include <features.h>

#define LC_ALL          0
#define LC_COLLATE      1
#define LC_CTYPE        2
#define LC_MONETARY     3
#define LC_NUMERIC      4
#define LC_TIME         5

#if (defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 1L) || defined(_DARWIN_C_SOURCE)
#define LC_MESSAGES     6
#endif /* (_POSIX_C_SOURCE &&  _POSIX_C_SOURCE >= 1) || _DARWIN_C_SOURCE */

struct lconv {
    /* LC_NUMERIC */
    char *decimal_point;
    char *thousands_sep;
    char *grouping;

    /* LC_MONETARY */
    char *int_curr_symbol;
    char *currency_symbol;
    char *mon_decimal_point;
    char *mon_thousands_sep;
    char *mon_grouping;
    char *positive_sign;
    char *negative_sign;
    char int_frac_digits;
    char frac_digits;
    char p_cs_precedes;
    char p_sep_by_space;
    char n_cs_precedes;
    char n_sep_by_space;
    char p_sign_posn;
    char n_sign_posn;
};

char* setlocale(int category, const char *locale);
struct lconv* localeconv(void);

#if defined(_DARWIN_C_SOURCE)

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

int setrlocale(const char *locale);

extern const RuneLocale _DefaultRuneLocale;
extern RuneLocale* _CurrentRuneLocale;

int rl_istype(RuneLocale *rl, int c, unsigned long f);
int rl_isalnum(RuneLocale *rl, int c);
int rl_isalpha(RuneLocale *rl, int c);
int rl_isblank(RuneLocale *rl, int c);
int rl_iscntrl(RuneLocale *rl, int c);
int rl_isdigit(RuneLocale *rl, int c);
int rl_isgraph(RuneLocale *rl, int c);
int rl_islower(RuneLocale *rl, int c);
int rl_isprint(RuneLocale *rl, int c);
int rl_ispunct(RuneLocale *rl, int c);
int rl_isspace(RuneLocale *rl, int c);
int rl_isupper(RuneLocale *rl, int c);
int rl_isxdigit(RuneLocale *rl, int c);

#endif /* _DARWIN_C_SOURCE */

#endif /* !LOCALE_H */
