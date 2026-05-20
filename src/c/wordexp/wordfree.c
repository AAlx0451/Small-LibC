#include <stdlib.h>
#include <wordexp.h>

void wordfree(wordexp_t *we)
{
    if (!we || !we->we_wordv) {
        return;
    }

    char **current_word = we->we_wordv + we->we_offs;
    char **end_word = current_word + we->we_wordc;

    while (current_word < end_word) {
        free(*current_word);
        current_word++;
    }

    free(we->we_wordv);
    we->we_wordv = NULL;
    we->we_wordc = 0;
}
