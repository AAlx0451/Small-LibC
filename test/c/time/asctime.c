#include <stdio.h>
#include <string.h>
#include <time.h>

#define RED(txt)   "\033[0;31m" txt "\033[0m"
#define GREEN(txt) "\033[0;32m" txt "\033[0m"

static int check_asctime(const struct tm *tm, const char *expected)
{
    char *result = asctime(tm);
    return (result != NULL) && (strcmp(result, expected) == 0);
}

void asctime_test(void)
{
    struct tm test_tm;
    char *result;

    memset(&test_tm, 0, sizeof(test_tm));
    test_tm.tm_sec = 52;
    test_tm.tm_min = 3;
    test_tm.tm_hour = 1;
    test_tm.tm_mday = 16;
    test_tm.tm_mon = 8;   // September (0-11)
    test_tm.tm_year = 73; // 1973
    test_tm.tm_wday = 0;  // Sunday

    if (check_asctime(&test_tm, "Sun Sep 16 01:03:52 1973\n")) {
        printf(GREEN("[PASS]") " asctime formats basic date correctly\n");
    } else {
        printf(RED("[FAIL]") " asctime formats basic date correctly\n");
    }

    test_tm.tm_mday = 5;
    if (check_asctime(&test_tm, "Sun Sep  5 01:03:52 1973\n")) {
        printf(GREEN("[PASS]") " asctime pads single-digit day with space\n");
    } else {
        printf(RED("[FAIL]") " asctime pads single-digit day with space\n");
    }

    test_tm.tm_mday = 25;
    if (check_asctime(&test_tm, "Sun Sep 25 01:03:52 1973\n")) {
        printf(GREEN("[PASS]") " asctime formats double-digit day without padding\n");
    } else {
        printf(RED("[FAIL]") " asctime formats double-digit day without padding\n");
    }

    test_tm.tm_hour = 0;
    test_tm.tm_min = 0;
    test_tm.tm_sec = 0;
    test_tm.tm_mday = 1;
    if (check_asctime(&test_tm, "Sun Sep  1 00:00:00 1973\n")) {
        printf(GREEN("[PASS]") " asctime handles zero time values\n");
    } else {
        printf(RED("[FAIL]") " asctime handles zero time values\n");
    }

    test_tm.tm_hour = 23;
    test_tm.tm_min = 59;
    test_tm.tm_sec = 59;
    if (check_asctime(&test_tm, "Sun Sep  1 23:59:59 1973\n")) {
        printf(GREEN("[PASS]") " asctime handles max time values\n");
    } else {
        printf(RED("[FAIL]") " asctime handles max time values\n");
    }

    test_tm.tm_wday = 6; // Saturday
    test_tm.tm_mon = 0;  // January
    test_tm.tm_mday = 15;
    if (check_asctime(&test_tm, "Sat Jan 15 23:59:59 1973\n")) {
        printf(GREEN("[PASS]") " asctime uses correct weekday abbreviation\n");
    } else {
        printf(RED("[FAIL]") " asctime uses correct weekday abbreviation\n");
    }

    test_tm.tm_mon = 11; // December
    if (check_asctime(&test_tm, "Sat Dec 15 23:59:59 1973\n")) {
        printf(GREEN("[PASS]") " asctime uses correct month abbreviation\n");
    } else {
        printf(RED("[FAIL]") " asctime uses correct month abbreviation\n");
    }

    test_tm.tm_year = 123;
    test_tm.tm_mon = 0;
    test_tm.tm_mday = 1;
    test_tm.tm_wday = 0;
    if (check_asctime(&test_tm, "Sun Jan  1 23:59:59 2023\n")) {
        printf(GREEN("[PASS]") " asctime handles year >= 2000\n");
    } else {
        printf(RED("[FAIL]") " asctime handles year >= 2000\n");
    }

    result = asctime(&test_tm);
    if (result != NULL && result == asctime(&test_tm)) {
        printf(GREEN("[PASS]") " asctime returns pointer to static buffer\n");
    } else {
        printf(RED("[FAIL]") " asctime returns pointer to static buffer\n");
    }

    if (result != NULL && strlen(result) == 25) {
        printf(GREEN("[PASS]") " asctime result length is 25 chars (+ \\n + \\0)\n");
    } else {
        printf(RED("[FAIL]") " asctime result length is 25 chars (+ \\n + \\0)\n");
    }

    if (result != NULL && result[24] == '\n' && result[25] == '\0') {
        printf(GREEN("[PASS]") " asctime terminates with \\n\\0\n");
    } else {
        printf(RED("[FAIL]") " asctime terminates with \\n\\0\n");
    }

    test_tm.tm_year = 70; // 1970
    char *first = asctime(&test_tm);
    test_tm.tm_year = 80; // 1980
    char *second = asctime(&test_tm);
    if (first == second && strcmp(first, "Sun Jan  1 23:59:59 1980\n") == 0) {
        printf(GREEN("[PASS]") " asctime static buffer is overwritten on next call\n");
    } else {
        printf(RED("[FAIL]") " asctime static buffer is overwritten on next call\n");
    }
}
