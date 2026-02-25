#include <ctype.h>
#include <fnmatch.h>
#include <string.h>

static int check_class(const char *name, size_t len, char c) {
    if(len == 5 && strncmp(name, "alnum", 5) == 0)
        return isalnum((unsigned char)c);
    if(len == 5 && strncmp(name, "alpha", 5) == 0)
        return isalpha((unsigned char)c);
    if(len == 5 && strncmp(name, "blank", 5) == 0)
        return (c == ' ' || c == '\t');
    if(len == 5 && strncmp(name, "cntrl", 5) == 0)
        return iscntrl((unsigned char)c);
    if(len == 5 && strncmp(name, "digit", 5) == 0)
        return isdigit((unsigned char)c);
    if(len == 5 && strncmp(name, "graph", 5) == 0)
        return isgraph((unsigned char)c);
    if(len == 5 && strncmp(name, "lower", 5) == 0)
        return islower((unsigned char)c);
    if(len == 5 && strncmp(name, "print", 5) == 0)
        return isprint((unsigned char)c);
    if(len == 5 && strncmp(name, "punct", 5) == 0)
        return ispunct((unsigned char)c);
    if(len == 5 && strncmp(name, "space", 5) == 0)
        return isspace((unsigned char)c);
    if(len == 5 && strncmp(name, "upper", 5) == 0)
        return isupper((unsigned char)c);
    if(len == 6 && strncmp(name, "xdigit", 6) == 0)
        return isxdigit((unsigned char)c);
    return 0;
}

static int check_range(char c, char start, char end) {
    char s_c[2] = {c, '\0'};
    char s_start[2] = {start, '\0'};
    char s_end[2] = {end, '\0'};
    return (strcoll(s_start, s_c) <= 0 && strcoll(s_c, s_end) <= 0);
}

static int fnmatch_internal(const char *pattern, const char *string, int flags, int is_start) {
    const char *p = pattern;
    const char *s = string;
    char c, start_char, end_char;

    while((c = *p++) != '\0') {
        if(!(flags & FNM_NOESCAPE) && c == '\\') {
            if(*p == '\0')
                return FNM_NOMATCH;
            c = *p++;
            if(c != *s)
                return FNM_NOMATCH;
            if((flags & FNM_PATHNAME) && c == '/')
                is_start = 1;
            else
                is_start = 0;
            s++;
            continue;
        }

        if(c == '[') {
            const char *start_p = p - 1;
            int negate = 0;
            const char *scan_end;
            int found = 0;
            const char *scan;

            if(*p == '!' || *p == '^') {
                negate = 1;
                p++;
            }

            scan_end = p;
            if(*scan_end == ']' || *scan_end == '-')
                scan_end++;
            while(*scan_end != '\0' && *scan_end != ']') {
                if(*scan_end == '[' && (scan_end[1] == ':' || scan_end[1] == '.' || scan_end[1] == '=')) {
                    char t = scan_end[1];
                    scan_end += 2;
                    while(*scan_end != '\0' && !(*scan_end == t && scan_end[1] == ']'))
                        scan_end++;
                    if(*scan_end != '\0')
                        scan_end += 2;
                } else {
                    scan_end++;
                }
            }

            if(*scan_end != ']') {
                if(*s != '[')
                    return FNM_NOMATCH;
                return fnmatch_internal(start_p + 1, s + 1, flags, ((flags & FNM_PATHNAME) && *s == '/') ? 1 : 0);
            }

            if((flags & FNM_PATHNAME) && *s == '/')
                return FNM_NOMATCH;
            if((flags & FNM_PERIOD) && is_start && *s == '.')
                return FNM_NOMATCH;
            if(*s == '\0')
                return FNM_NOMATCH;

            scan = p;
            if(*scan == ']' || *scan == '-') {
                if(*s == *scan)
                    found = 1;
                scan++;
            }

            while(scan < scan_end) {
                if(*scan == '[' && (scan[1] == ':' || scan[1] == '.' || scan[1] == '=')) {
                    char type = scan[1];
                    const char *c_end = scan + 2;
                    while(*c_end != '\0' && !(*c_end == type && c_end[1] == ']'))
                        c_end++;
                    if(*c_end != '\0') {
                        if(type == ':') {
                            if(check_class(scan + 2, (size_t)(c_end - (scan + 2)), *s))
                                found = 1;
                        } else if(type == '=' || type == '.') {
                            if(c_end - (scan + 2) == 1 && *s == scan[2])
                                found = 1;
                        }
                        scan = c_end + 2;
                        continue;
                    }
                }

                start_char = *scan++;
                if(scan < scan_end && *scan == '-' && (scan + 1) < scan_end) {
                    scan++;
                    end_char = *scan++;
                    if(check_range(*s, start_char, end_char))
                        found = 1;
                } else {
                    if(*s == start_char)
                        found = 1;
                }
            }

            if(negate)
                found = !found;
            if(!found)
                return FNM_NOMATCH;

            return fnmatch_internal(scan_end + 1, s + 1, flags, 0);
        }

        if(c == '?') {
            if(*s == '\0')
                return FNM_NOMATCH;
            if((flags & FNM_PATHNAME) && *s == '/')
                return FNM_NOMATCH;
            if((flags & FNM_PERIOD) && is_start && *s == '.')
                return FNM_NOMATCH;
            s++;
            is_start = 0;
            continue;
        }

        if(c == '*') {
            while(*p == '*')
                p++;

            if((flags & FNM_PERIOD) && is_start && *s == '.') {
                if(*p == '\0')
                    return FNM_NOMATCH;
                return fnmatch_internal(p, s, flags, is_start);
            }

            if(*p == '\0') {
                if(flags & FNM_PATHNAME) {
                    for(; *s != '\0'; s++) {
                        if(*s == '/')
                            return FNM_NOMATCH;
                    }
                }
                return 0;
            }

            while(1) {
                if(fnmatch_internal(p, s, flags, is_start) == 0)
                    return 0;
                if(*s == '\0')
                    break;
                if((flags & FNM_PATHNAME) && *s == '/')
                    break;
                s++;
                is_start = 0;
            }
            return FNM_NOMATCH;
        }

        if(c != *s)
            return FNM_NOMATCH;

        if((flags & FNM_PATHNAME) && c == '/')
            is_start = 1;
        else
            is_start = 0;

        s++;
    }

    if(*s == '\0')
        return 0;
    return FNM_NOMATCH;
}

int fnmatch(const char *pattern, const char *string, int flags) {
    return fnmatch_internal(pattern, string, flags, 1);
}
