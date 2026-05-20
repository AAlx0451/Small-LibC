#include "regex_internal.h"

size_t regerror(int errcode, __unused const regex_t *preg, char *errbuf, size_t errbuf_size)
{
    const char *msg = "Unknown error";
    switch (errcode) {
    case REG_NOMATCH:
        msg = "No match";
        break;
    case REG_BADPAT:
        msg = "Invalid regular expression";
        break;
    case REG_ECOLLATE:
        msg = "Invalid collating element";
        break;
    case REG_ECTYPE:
        msg = "Invalid character class";
        break;
    case REG_EESCAPE:
        msg = "Trailing backslash";
        break;
    case REG_ESUBREG:
        msg = "Invalid back reference";
        break;
    case REG_EBRACK:
        msg = "Unmatched [, [^, [:, [., or [=";
        break;
    case REG_EPAREN:
        msg = "Unmatched ( or \\(";
        break;
    case REG_EBRACE:
        msg = "Unmatched \\{";
        break;
    case REG_BADBR:
        msg = "Invalid \\{\\} contents";
        break;
    case REG_ERANGE:
        msg = "Invalid range end";
        break;
    case REG_ESPACE:
        msg = "Out of memory";
        break;
    case REG_BADRPT:
        msg = "Invalid preceding regular expression";
        break;
    }

    size_t len = 0;
    while (msg[len])
        len++;

    if (errbuf_size > 0 && errbuf) {
        size_t copy_len = len < errbuf_size - 1 ? len : errbuf_size - 1;
        for (size_t i = 0; i < copy_len; i++)
            errbuf[i] = msg[i];
        errbuf[copy_len] = '\0';
    }

    return len + 1;
}
