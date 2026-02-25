#include <locale.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static const char c_locale_name[] = "C";

static const char *get_env_name(int category) {
    switch(category) {
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

char *setlocale(int category, const char *locale) {
    const char *target;
    if(category < 0 || category > 6) {
        return NULL;
    }

    if(locale == NULL) {
        return (char *)(uintptr_t)c_locale_name;
    }

    target = locale;

    if(locale[0] == '\0') {
        target = getenv("LC_ALL");

        if(target == NULL || target[0] == '\0') {
            target = getenv(get_env_name(category));
        }

        if(target == NULL || target[0] == '\0') {
            target = getenv("LANG");
        }

        if(target == NULL || target[0] == '\0') {
            target = c_locale_name;
        }
    }

    if(strcmp(target, "C") == 0 || strcmp(target, "POSIX") == 0) {
        return (char *)(uintptr_t)c_locale_name;
    }

    return NULL;
}
