#ifndef LANGINFO_H
#define LANGINFO_H

#include <features.h>

#if !defined(_ANSI) &&                                                                             \
    ((defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 200112L) || defined(_XOPEN_SOURCE))

# include <nl_types.h>

# define CODESET 0

# define ABMON_1  1
# define ABMON_2  2
# define ABMON_3  3
# define ABMON_4  4
# define ABMON_5  5
# define ABMON_6  6
# define ABMON_7  7
# define ABMON_8  8
# define ABMON_9  9
# define ABMON_10 10
# define ABMON_11 11
# define ABMON_12 12

# define MON_1  13
# define MON_2  14
# define MON_3  15
# define MON_4  16
# define MON_5  17
# define MON_6  18
# define MON_7  19
# define MON_8  20
# define MON_9  21
# define MON_10 22
# define MON_11 23
# define MON_12 24

# define ABDAY_1 25
# define ABDAY_2 26
# define ABDAY_3 27
# define ABDAY_4 28
# define ABDAY_5 29
# define ABDAY_6 30
# define ABDAY_7 31

# define DAY_1 32
# define DAY_2 33
# define DAY_3 34
# define DAY_4 35
# define DAY_5 36
# define DAY_6 37
# define DAY_7 38

# define T_FMT      39
# define D_FMT      40
# define D_T_FMT    41
# define AM_STR     42
# define PM_STR     43
# define T_FMT_AMPM 58

# define RADIXCHAR 100
# define THOUSEP   101
# define CRNCYSTR  102

# define YESEXPR 200
# define NOEXPR  201

char *nl_langinfo(nl_item);

#endif /* !_ANSI && ((_POSIX_C_SOURCE && _POSIX_C_SOURCE >= 200112L) || _XOPEN_SOURCE) */

#endif /* !LANGINFO_H */
