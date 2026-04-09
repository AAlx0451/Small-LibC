/*
 * based on Yasuhiro Matsumoto's strtod
 * public domain
 */

#include <ctype.h>
#include <errno.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <locale.h>

double strtod(const char *str, char **end) {
    return (double)strtold(str, end);
}
