#include <string.h>
#include <wctype.h>

wctype_t wctype(const char *property) {
    if(strcmp(property, "alnum") == 0)
        return 1;
    if(strcmp(property, "alpha") == 0)
        return 2;
    if(strcmp(property, "blank") == 0)
        return 3;
    if(strcmp(property, "cntrl") == 0)
        return 4;
    if(strcmp(property, "digit") == 0)
        return 5;
    if(strcmp(property, "graph") == 0)
        return 6;
    if(strcmp(property, "lower") == 0)
        return 7;
    if(strcmp(property, "print") == 0)
        return 8;
    if(strcmp(property, "punct") == 0)
        return 9;
    if(strcmp(property, "space") == 0)
        return 10;
    if(strcmp(property, "upper") == 0)
        return 11;
    if(strcmp(property, "xdigit") == 0)
        return 12;
    return 0;
}
