#include <stdio.h>
#include <string.h>

#define RED(txt)   "\033[0;31m" txt "\033[0m"
#define GREEN(txt) "\033[0;32m" txt "\033[0m"

void strtok_test()
{
    char s1[] = "_a_bc__d_";
    char *ptr = strtok(s1, "_");

    if (ptr != &s1[1] || s1[1] != 'a' || s1[2] != '\0')
        printf(RED("[FAIL]") " strtok handles leading delimiters and first token correctly\n");
    else
        printf(GREEN("[PASS]") " strtok handles leading delimiters and first token correctly\n");

    ptr = strtok(NULL, "_");
    if (ptr != &s1[3] || s1[3] != 'b' || s1[4] != 'c' || s1[5] != '\0')
        printf(RED("[FAIL]") " strtok handles subsequent tokens correctly\n");
    else
        printf(GREEN("[PASS]") " strtok handles subsequent tokens correctly\n");

    ptr = strtok(NULL, "_");
    if (ptr != &s1[7] || s1[6] != '_' || s1[7] != 'd' || s1[8] != '\0' || strtok(NULL, "_") != NULL)
        printf(RED("[FAIL]") " strtok handles multiple delimiters and end of string correctly\n");
    else
        printf(GREEN("[PASS]") " strtok handles multiple delimiters and end of string correctly\n");

    char s2[] = "ab_cd";
    if (strtok(s2, "_") != &s2[0] || s2[2] != '\0' || strtok(NULL, "_") != &s2[3] ||
        strtok(NULL, "_") != NULL)
        printf(RED("[FAIL]") " strtok handles strings starting without delimiters correctly\n");
    else
        printf(GREEN("[PASS]") " strtok handles strings starting without delimiters correctly\n");
}
