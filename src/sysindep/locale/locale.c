#include <locale.h>
#include <string.h>
#include <limits.h>

static struct lconv c_locale_data;
static int is_initialized = 0;

static const char c_locale_name[] = "C";

char *setlocale(int category, const char *locale)
{
    if (category < LC_CTYPE || category > LC_ALL)
    {
        return NULL;
    }

    if (locale == NULL)
    {
        return (char*)c_locale_name;
    }

    if (strcmp(locale, "C") == 0 || strcmp(locale, "") == 0)
    {
        return (char*)c_locale_name;
    }
    
    return NULL;
}

struct lconv *localeconv(void)
{
    if (!is_initialized)
    {
        c_locale_data.decimal_point = ".";
        c_locale_data.thousands_sep = "";
        c_locale_data.grouping = "";
        c_locale_data.int_curr_symbol = "";
        c_locale_data.currency_symbol = "";
        c_locale_data.mon_decimal_point = "";
        c_locale_data.mon_thousands_sep = "";
        c_locale_data.mon_grouping = "";
        c_locale_data.positive_sign = "";
        c_locale_data.negative_sign = "";
        c_locale_data.int_frac_digits = CHAR_MAX;
        c_locale_data.frac_digits = CHAR_MAX;
        c_locale_data.p_cs_precedes = CHAR_MAX;
        c_locale_data.p_sep_by_space = CHAR_MAX;
        c_locale_data.n_cs_precedes = CHAR_MAX;
        c_locale_data.n_sep_by_space = CHAR_MAX;
        c_locale_data.p_sign_posn = CHAR_MAX;
        c_locale_data.n_sign_posn = CHAR_MAX;

        is_initialized = 1;
    }

    return &c_locale_data;
}
