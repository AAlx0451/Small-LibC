#include <locale.h>
#include <string.h>

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
