char *strpbrk(const char *s, const char *accept) {
    while (*s) {
        const char *a = accept;
        while (*a) {
            if (*a++ == *s)
                return (char *)s;
        }
        s++;
    }
    return 0;
}
