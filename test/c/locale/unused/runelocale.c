#include "runelocale.h"
#include <stdint.h>
#include <stdio.h>
#include <wchar.h>

#define RED(txt) "\033[0;31m" txt "\033[0m"
#define GREEN(txt) "\033[0;32m" txt "\033[0m"
#define YELLOW(txt) "\033[0;33m" txt "\033[0m"

void runelocale_test() {
#if !defined(_RL_USED)
    printf(YELLOW("[INFO]") " runelocale functions are currently unused\n");
#endif

    // --- 1. Memory and File Loading Tests ---
    RuneLocale *rl = rl_load_file("/usr/share/locale/en_US.UTF-8/LC_CTYPE");
    RuneLocale *bad_rl = rl_load_file("/invalid/path/to/locale");

    if(!rl)
        printf(RED("[FAIL]") " rl_load_file successfully loads a valid locale file\n");
    else
        printf(GREEN("[PASS]") " rl_load_file successfully loads a valid locale file\n");

    if(bad_rl)
        printf(RED("[FAIL]") " rl_load_file returns NULL for invalid paths\n");
    else
        printf(GREEN("[PASS]") " rl_load_file returns NULL for invalid paths\n");

    if(bad_rl)
        rl_free(bad_rl);

    const char bad_magic[3164] = "BadMagicStr"; // Minimum size to pass size check, but fails magic check
    RuneLocale *mem_rl = rl_load_mem(bad_magic, sizeof(bad_magic));
    if(mem_rl)
        printf(RED("[FAIL]") " rl_load_mem returns NULL for invalid magic signature\n");
    else
        printf(GREEN("[PASS]") " rl_load_mem returns NULL for invalid magic signature\n");

    if(mem_rl)
        rl_free(mem_rl);

    if(!rl) {
        printf(YELLOW("[SKIP]") " remaining tests require valid locale file\n");
        return;
    }

    // --- 2. Base runetype Mask Tests ---
    if(rl_runetype(rl, 'A') == 0 || rl_runetype(rl, '0') == 0 || rl_runetype(rl, ' ') == 0)
        printf(RED("[FAIL]") " rl_runetype returns non-zero mask for standard ASCII characters\n");
    else
        printf(GREEN("[PASS]") " rl_runetype returns non-zero mask for standard ASCII characters\n");

    if(rl_runetype(rl, EOF) != 0 || rl_runetype(rl, -5) != 0)
        printf(RED("[FAIL]") " rl_runetype returns 0 for EOF and negative out-of-bounds values\n");
    else
        printf(GREEN("[PASS]") " rl_runetype returns 0 for EOF and negative out-of-bounds values\n");

    // --- 3. toupper / tolower Tests ---
    if(rl_toupper(rl, 'a') != 'A' || rl_toupper(rl, 'z') != 'Z' || rl_toupper(rl, 'A') != 'A' || rl_toupper(rl, '1') != '1')
        printf(RED("[FAIL]") " rl_toupper handles ASCII characters correctly\n");
    else
        printf(GREEN("[PASS]") " rl_toupper handles ASCII characters correctly\n");

    if(rl_toupper(rl, L'ѳ') != L'Ѳ' || rl_toupper(rl, L'æ') != L'Æ' || rl_toupper(rl, L'Æ') != L'Æ' || rl_toupper(rl, L'¡') != L'¡')
        printf(RED("[FAIL]") " rl_toupper handles extended Unicode characters correctly\n");
    else
        printf(GREEN("[PASS]") " rl_toupper handles extended Unicode characters correctly\n");

    if(rl_toupper(rl, EOF) != EOF || rl_toupper(rl, -5) != -5)
        printf(RED("[FAIL]") " rl_toupper leaves EOF and negative values unchanged\n");
    else
        printf(GREEN("[PASS]") " rl_toupper leaves EOF and negative values unchanged\n");

    if(rl_tolower(rl, 'A') != 'a' || rl_tolower(rl, 'Z') != 'z' || rl_tolower(rl, 'a') != 'a' || rl_tolower(rl, '1') != '1')
        printf(RED("[FAIL]") " rl_tolower handles ASCII characters correctly\n");
    else
        printf(GREEN("[PASS]") " rl_tolower handles ASCII characters correctly\n");

    if(rl_tolower(rl, L'Ѳ') != L'ѳ' || rl_tolower(rl, L'Æ') != L'æ' || rl_tolower(rl, L'æ') != L'æ' || rl_tolower(rl, L'¡') != L'¡')
        printf(RED("[FAIL]") " rl_tolower handles extended Unicode characters correctly\n");
    else
        printf(GREEN("[PASS]") " rl_tolower handles extended Unicode characters correctly\n");

    if(rl_tolower(rl, EOF) != EOF || rl_tolower(rl, -5) != -5)
        printf(RED("[FAIL]") " rl_tolower leaves EOF and negative values unchanged\n");
    else
        printf(GREEN("[PASS]") " rl_tolower leaves EOF and negative values unchanged\n");

    // --- 4. Character Class Classification Tests (Inlines) ---
    if(!rl_isascii('a') || !rl_isascii('\n') || !rl_isascii(127) || rl_isascii(128) || rl_isascii(EOF))
        printf(RED("[FAIL]") " rl_isascii correctly identifies 7-bit ASCII boundaries\n");
    else
        printf(GREEN("[PASS]") " rl_isascii correctly identifies 7-bit ASCII boundaries\n");

    if(!rl_isalnum(rl, 'A') || !rl_isalnum(rl, 'z') || !rl_isalnum(rl, '5') || !rl_isalnum(rl, L'ѳ') || rl_isalnum(rl, ' ') || rl_isalnum(rl, '!'))
        printf(RED("[FAIL]") " rl_isalnum identifies alphanumeric characters correctly\n");
    else
        printf(GREEN("[PASS]") " rl_isalnum identifies alphanumeric characters correctly\n");

    if(!rl_isalpha(rl, 'a') || !rl_isalpha(rl, 'Z') || !rl_isalpha(rl, L'ѳ') || rl_isalpha(rl, '1') || rl_isalpha(rl, ' ') || rl_isalpha(rl, EOF))
        printf(RED("[FAIL]") " rl_isalpha identifies alphabetic characters correctly\n");
    else
        printf(GREEN("[PASS]") " rl_isalpha identifies alphabetic characters correctly\n");

    if(!rl_isblank(rl, ' ') || !rl_isblank(rl, '\t') || rl_isblank(rl, '\n') || rl_isblank(rl, 'a'))
        printf(RED("[FAIL]") " rl_isblank identifies blank characters (space, tab) correctly\n");
    else
        printf(GREEN("[PASS]") " rl_isblank identifies blank characters (space, tab) correctly\n");

    if(!rl_iscntrl(rl, '\0') || !rl_iscntrl(rl, '\n') || !rl_iscntrl(rl, 127) || rl_iscntrl(rl, 'a') || rl_iscntrl(rl, ' '))
        printf(RED("[FAIL]") " rl_iscntrl identifies control characters correctly\n");
    else
        printf(GREEN("[PASS]") " rl_iscntrl identifies control characters correctly\n");

    if(!rl_isdigit(rl, '0') || !rl_isdigit(rl, '9') || rl_isdigit(rl, 'a') || rl_isdigit(rl, ' ') || rl_isdigit(rl, L'ѳ'))
        printf(RED("[FAIL]") " rl_isdigit identifies decimal digits correctly\n");
    else
        printf(GREEN("[PASS]") " rl_isdigit identifies decimal digits correctly\n");

    if(!rl_isgraph(rl, 'a') || !rl_isgraph(rl, '1') || !rl_isgraph(rl, '!') || rl_isgraph(rl, ' ') || rl_isgraph(rl, '\n'))
        printf(RED("[FAIL]") " rl_isgraph identifies graphical characters correctly\n");
    else
        printf(GREEN("[PASS]") " rl_isgraph identifies graphical characters correctly\n");

    if(!rl_islower(rl, 'a') || !rl_islower(rl, 'z') || !rl_islower(rl, L'ѳ') || rl_islower(rl, 'A') || rl_islower(rl, '1'))
        printf(RED("[FAIL]") " rl_islower identifies lowercase characters correctly\n");
    else
        printf(GREEN("[PASS]") " rl_islower identifies lowercase characters correctly\n");

    if(!rl_isprint(rl, 'a') || !rl_isprint(rl, ' ') || !rl_isprint(rl, '1') || !rl_isprint(rl, L'ѳ') || rl_isprint(rl, '\a') || rl_isprint(rl, EOF))
        printf(RED("[FAIL]") " rl_isprint identifies printable characters correctly\n");
    else
        printf(GREEN("[PASS]") " rl_isprint identifies printable characters correctly\n");

    if(!rl_ispunct(rl, '!') || !rl_ispunct(rl, '.') || !rl_ispunct(rl, ',') || rl_ispunct(rl, 'a') || rl_ispunct(rl, '1') || rl_ispunct(rl, ' '))
        printf(RED("[FAIL]") " rl_ispunct identifies punctuation characters correctly\n");
    else
        printf(GREEN("[PASS]") " rl_ispunct identifies punctuation characters correctly\n");

    if(!rl_isspace(rl, ' ') || !rl_isspace(rl, '\t') || !rl_isspace(rl, '\n') || !rl_isspace(rl, '\r') || !rl_isspace(rl, '\v') || !rl_isspace(rl, '\f') || rl_isspace(rl, 'a') || rl_isspace(rl, '1'))
        printf(RED("[FAIL]") " rl_isspace identifies whitespace characters correctly\n");
    else
        printf(GREEN("[PASS]") " rl_isspace identifies whitespace characters correctly\n");

    if(!rl_isupper(rl, 'A') || !rl_isupper(rl, 'Z') || !rl_isupper(rl, L'Ѳ') || rl_isupper(rl, 'a') || rl_isupper(rl, '1') || rl_isupper(rl, L'ѳ'))
        printf(RED("[FAIL]") " rl_isupper identifies uppercase characters correctly\n");
    else
        printf(GREEN("[PASS]") " rl_isupper identifies uppercase characters correctly\n");

    if(!rl_isxdigit(rl, '0') || !rl_isxdigit(rl, '9') || !rl_isxdigit(rl, 'a') || !rl_isxdigit(rl, 'f') || !rl_isxdigit(rl, 'A') || !rl_isxdigit(rl, 'F') || rl_isxdigit(rl, 'g') || rl_isxdigit(rl, 'G') || rl_isxdigit(rl, ' '))
        printf(RED("[FAIL]") " rl_isxdigit identifies hexadecimal digits correctly\n");
    else
        printf(GREEN("[PASS]") " rl_isxdigit identifies hexadecimal digits correctly\n");

    rl_free(rl);
}
