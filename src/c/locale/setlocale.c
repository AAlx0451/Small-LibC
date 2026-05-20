#include <locale.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static char *active_locales[7] = {NULL};
static const char c_locale_name[] = "C";
static char ret_buf[256];

static const char *get_env_name(int category)
{
    switch (category) {
    case LC_ALL:
        return "LC_ALL";
    case LC_CTYPE:
        return "LC_CTYPE";
    case LC_COLLATE:
        return "LC_COLLATE";
    case LC_TIME:
        return "LC_TIME";
    case LC_NUMERIC:
        return "LC_NUMERIC";
    case LC_MONETARY:
        return "LC_MONETARY";
    case LC_MESSAGES:
        return "LC_MESSAGES";
    default:
        return NULL;
    }
}

char *setlocale(int category, const char *locale)
{
    const char *target;
    int i, start, end;

    if (category < 0 || category > 6) {
        return NULL;
    }

    if (locale == NULL) {
        if (category == LC_ALL) {
            for (i = 0; i <= 6; i++) {
                if (i != LC_ALL && active_locales[i]) {
                    return active_locales[i];
                }
            }
            return __deconst(char *, c_locale_name);
        }
        return active_locales[category] ? active_locales[category]
                                        : __deconst(char *, c_locale_name);
    }

    target = locale;
    if (locale[0] == '\0') {
        target = getenv("LC_ALL");
        if (!target || target[0] == '\0') {
            target = getenv(get_env_name(category));
        }
        if (!target || target[0] == '\0') {
            target = getenv("LANG");
        }
        if (!target || target[0] == '\0') {
            target = c_locale_name;
        }
    }

    if (strcmp(target, "C") == 0 || strcmp(target, "POSIX") == 0) {
        target = c_locale_name;
    }

    start = (category == LC_ALL) ? 0 : category;
    end = (category == LC_ALL) ? 6 : category;

    for (i = start; i <= end; i++) {
        if (i == LC_ALL && category == LC_ALL)
            continue;

        if (i == LC_CTYPE) {
            if (setrlocale(target) != 0)
                return NULL;
        }
        if (active_locales[i] && active_locales[i] != c_locale_name) {
            free(active_locales[i]);
        }

        if (target == c_locale_name) {
            active_locales[i] = __deconst(char *, c_locale_name);
        } else {
            active_locales[i] = strdup(target);
        }
    }

    strncpy(ret_buf, target, sizeof(ret_buf) - 1);
    ret_buf[sizeof(ret_buf) - 1] = '\0';
    return ret_buf;
}
