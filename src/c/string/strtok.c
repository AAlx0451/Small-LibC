static char *olds;

char *strtok(char *s, const char *delim) {
    char *token;
    if (s == 0)
        s = olds;
    while (*s) {
        const char *d = delim;
        while (*d) {
            if (*s == *d) break;
            d++;
        }
        if (!*d) break;
        s++;
    }
    if (!*s) {
        olds = s;
        return 0;
    }
    token = s;
    while (*s) {
        const char *d = delim;
        while (*d) {
            if (*s == *d) {
                *s = '\0';
                olds = s + 1;
                return token;
            }
            d++;
        }
        s++;
    }
    olds = s;
    return token;
}
