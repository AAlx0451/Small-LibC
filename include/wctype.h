#ifndef WCTYPE_H
#define WCTYPE_H

#include <features.h>

#if !defined(_ANSI) && ((defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 200112L) ||                \
                        (defined(_XOPEN_SOURCE) && _XOPEN_SOURCE >= 600) ||                        \
                        (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199409LL))

# include <stddef.h>
# include <wchar.h>

typedef unsigned int wctype_t;
typedef unsigned int wctrans_t;

int iswalnum(wint_t wc);
int iswalpha(wint_t wc);
int iswcntrl(wint_t wc);
int iswdigit(wint_t wc);
int iswgraph(wint_t wc);
int iswlower(wint_t wc);
int iswprint(wint_t wc);
int iswpunct(wint_t wc);
int iswspace(wint_t wc);
int iswupper(wint_t wc);
int iswxdigit(wint_t wc);

# if (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L) || _POSIX_C_SOURCE >= 200112L ||   \
     _XOPEN_SOURCE >= 600
int iswblank(wint_t wc);
# endif /* (__STDC_VERSION__ && __STDC_VERSION__ >= 199001L) || _POSIX_C_SOURCE >= 200112L ||      \
           _XOPEN_SOURCE >= 600 */

wctype_t wctype(const char *property);
int iswctype(wint_t wc, wctype_t desc);

wint_t towlower(wint_t wc);
wint_t towupper(wint_t wc);

wctrans_t wctrans(const char *property);
wint_t towctrans(wint_t wc, wctrans_t desc);

#endif /* !_ANSI && ((_POSIX_C_SOURCE && _POSIX_C_SOURCE >= 200112L) || _XOPEN_SOURCE &&           \
          _XOPEN_SOURCE >= 600) || (__STDC_VERSION__ && __STDC_VERSION__ >= 199409L)) */

#endif /* !WCTYPE_H */
