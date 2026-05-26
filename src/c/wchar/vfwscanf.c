#include <ctype.h>
#include <limits.h>
#include <locale.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wchar.h>
#include <wctype.h>

#pragma clang diagnostic ignored "-Wunknown-warning-option"
#pragma clang diagnostic ignored "-Wreserved-identifier"

#define MAX_SCAN_WIDTH 256

#define FL_SPLAT      0x01
#define FL_WIDTH      0x02
#define FL_LONG       0x04
#define FL_LONGLONG   0x08
#define FL_SHORT      0x10
#define FL_SHORTSHORT 0x20
#define FL_LONGDOUBLE 0x40
#define FL_INTMAX     0x80
#define FL_SIZE       0x100
#define FL_PTRDIFF    0x200

/* Read a single wide character atomically from stream without internal locks */
static wint_t _in_wchar(FILE *f)
{
    if (f->_flags & __S_STR) {
        if (f->_cnt > 0) {
            f->_cnt--;
            wchar_t wc;
            memcpy(&wc, f->_ptr, sizeof(wc));
            f->_ptr += sizeof(wchar_t);
            return (wint_t)wc;
        }
        return WEOF;
    }

    wchar_t wc;
    size_t res;
    int byte_read;
    char c_byte;
    size_t bytes_consumed = 0;

    while (1) {
        if (f->_cnt > 0) {
            byte_read = *f->_ptr++;
            f->_cnt--;
        } else {
            if (__stdio_fill_impl(f) == 0) {
                if (f->_cnt > 0) {
                    byte_read = *f->_ptr++;
                    f->_cnt--;
                } else {
                    byte_read = EOF;
                }
            } else {
                byte_read = EOF;
            }
        }

        if (byte_read == EOF) {
            if (bytes_consumed == 0)
                return WEOF;
            f->_flags |= __S_ERR;
            return WEOF;
        }

        c_byte = (char)byte_read;
        bytes_consumed++;

        res = mbrtowc(&wc, &c_byte, 1, &f->_mbstate);
        if (res == (size_t)-1) {
            f->_flags |= __S_ERR;
            return WEOF;
        }
        if (res == (size_t)-2)
            continue;
        break;
    }
    return (wint_t)wc;
}

static void _unget_wchar(FILE *f, wint_t wc)
{
    if (wc == WEOF)
        return;

    if (f->_flags & __S_STR) {
        if (f->_ptr > f->_base) {
            f->_ptr -= sizeof(wchar_t);
            f->_cnt++;
        }
        return;
    }

    char buf[MB_LEN_MAX];
    mbstate_t temp_mbs = {0};
    size_t len = wcrtomb(buf, (wchar_t)wc, &temp_mbs);

    if (len == (size_t)-1)
        return;

    int reserve_size = (f->_flags & __S_RESERVE) ? MB_LEN_MAX : 0;
    unsigned char *min_ptr = f->_base - reserve_size;

    if (f->_base && (f->_ptr - len) >= min_ptr) {
        f->_ptr -= len;
        for (size_t i = 0; i < len; i++)
            f->_ptr[i] = (unsigned char)buf[i];
        f->_cnt += len;
        f->_flags &= ~__S_EOF;
    }
}

static wint_t _skip_whitespace_wc(FILE *f, int *consumed)
{
    wint_t c;
    while ((c = _in_wchar(f)) != WEOF) {
        (*consumed)++;
        if (!iswspace(c))
            return c;
    }
    return WEOF;
}

/* Helper for %[] to match ranges like a-z without allocating 1MB arrays */
static int _match_scanset(wint_t c, const wchar_t *start, const wchar_t *end)
{
    const wchar_t *p = start;
    while (p < end) {
        if (p + 2 < end && p[1] == L'-' && p[2] != L']') {
            if (c >= *p && c <= p[2])
                return 1;
            p += 3;
        } else {
            if (c == *p)
                return 1;
            p++;
        }
    }
    return 0;
}

int vfwscanf(FILE *restrict stream, const wchar_t *restrict format, va_list arg)
{
    struct lconv *lc = localeconv();
    const char *dp = (lc && lc->decimal_point && lc->decimal_point[0]) ? lc->decimal_point : ".";
    const wchar_t *p = format;
    int m, val, nmatch = 0, width, flags, i, chars_consumed = 0, digits, read_count, matched;
    wint_t c, conversion_char;
    char buf[MAX_SCAN_WIDTH + 1];

    if (!stream || !format)
        return EOF;

    int mode = fwide(stream, 0);
    if (mode < 0)
        return EOF;
    else if (mode == 0)
        fwide(stream, 1);

    if (!(stream->_flags & __S_STR)) {
        _spin_lock(&stream->_lock);
        if (stream->_flags & __S_DIRTY) {
            if (__stdio_flush_impl(stream) == EOF) {
                stream->_flags |= __S_ERR;
                _spin_unlock(&stream->_lock);
                return EOF;
            }
        }
        if (stream->_flags & __S_WR) {
            stream->_flags &= ~__S_WR;
            stream->_cnt = 0;
            stream->_ptr = stream->_base;
        }
        if (!(stream->_flags & __S_RD)) {
            stream->_flags |= __S_RD;
            if (!(stream->_flags & __S_NBF)) {
                stream->_cnt = 0;
                stream->_ptr = stream->_base;
            }
        }
    }

    while (*p) {
        if (iswspace(*p)) {
            while (iswspace(*p))
                p++;
            c = _skip_whitespace_wc(stream, &chars_consumed);
            _unget_wchar(stream, c);
            if (c != WEOF)
                chars_consumed--;
            continue;
        }

        if (*p != L'%') {
            c = _in_wchar(stream);
            if (c != WEOF)
                chars_consumed++;
            if (c == WEOF)
                goto input_failure;
            if (c != *p) {
                _unget_wchar(stream, c);
                if (c != WEOF)
                    chars_consumed--;
                goto match_failure;
            }
            p++;
            continue;
        }

        p++;
        if (*p == L'%') {
            c = _in_wchar(stream);
            if (c != WEOF)
                chars_consumed++;
            if (c == WEOF)
                goto input_failure;
            if (c != L'%') {
                _unget_wchar(stream, c);
                if (c != WEOF)
                    chars_consumed--;
                goto match_failure;
            }
            p++;
            continue;
        }

        flags = 0;
        width = 0;

        if (*p == L'*') {
            flags |= FL_SPLAT;
            p++;
        }

        if (iswdigit(*p)) {
            flags |= FL_WIDTH;
            while (iswdigit(*p)) {
                width = width * 10 + (*p - L'0');
                p++;
            }
            if (width == 0)
                width = 1;
        } else
            width = -1;

        /* Parse length modifiers including C99 j, z, t */
        if (*p == L'h') {
            p++;
            if (*p == L'h') {
                flags |= FL_SHORTSHORT;
                p++;
            } else {
                flags |= FL_SHORT;
            }
        } else if (*p == L'l') {
            p++;
            if (*p == L'l') {
                flags |= FL_LONGLONG;
                p++;
            } else {
                flags |= FL_LONG;
            }
        } else if (*p == L'L') {
            flags |= FL_LONGDOUBLE;
            p++;
        } else if (*p == L'j') {
            flags |= FL_INTMAX;
            p++;
        } else if (*p == L'z') {
            flags |= FL_SIZE;
            p++;
        } else if (*p == L't') {
            flags |= FL_PTRDIFF;
            p++;
        }

        conversion_char = *p++;

        if (conversion_char != L'c' && conversion_char != L'C' && conversion_char != L's' &&
            conversion_char != L'S' && conversion_char != L'[' && conversion_char != L'n') {
            c = _skip_whitespace_wc(stream, &chars_consumed);
            if (c == WEOF)
                goto input_failure;
            _unget_wchar(stream, c);
            if (c != WEOF)
                chars_consumed--;
        }

        switch (conversion_char) {
        case L'n':
            if (!(flags & FL_SPLAT)) {
                if (flags & FL_LONGLONG)
                    *va_arg(arg, long long *) = chars_consumed;
                else if (flags & FL_LONG)
                    *va_arg(arg, long *) = chars_consumed;
                else if (flags & FL_SHORT)
                    *va_arg(arg, short *) = (short)chars_consumed;
                else if (flags & FL_SHORTSHORT)
                    *va_arg(arg, char *) = (char)chars_consumed;
                else if (flags & FL_INTMAX)
                    *va_arg(arg, intmax_t *) = chars_consumed;
                else if (flags & FL_SIZE)
                    *va_arg(arg, ssize_t *) = chars_consumed;
                else if (flags & FL_PTRDIFF)
                    *va_arg(arg, ptrdiff_t *) = chars_consumed;
                else
                    *va_arg(arg, int *) = chars_consumed;
            }
            continue;

        case L'd':
        case L'i':
        case L'u':
        case L'x':
        case L'X':
        case L'o':
        case L'p': {
            int base = 10;
            int is_neg = 0;
            int has_digit = 0;
            if (conversion_char == L'i')
                base = 0;
            else if (conversion_char == L'x' || conversion_char == L'X' || conversion_char == L'p')
                base = 16;
            else if (conversion_char == L'o')
                base = 8;

            c = _in_wchar(stream);
            if (c != WEOF)
                chars_consumed++;
            if (c == WEOF)
                goto input_failure;

            if (c == L'-') {
                is_neg = 1;
                c = _in_wchar(stream);
                if (c != WEOF)
                    chars_consumed++;
                if (width > 0)
                    width--;
            } else if (c == L'+') {
                c = _in_wchar(stream);
                if (c != WEOF)
                    chars_consumed++;
                if (width > 0)
                    width--;
            }

            i = 0;
            if (is_neg)
                buf[i++] = '-';

            if (c == L'0') {
                has_digit = 1;
                if (width != 0 && (base == 0 || base == 16)) {
                    wint_t next = _in_wchar(stream);
                    if (next != WEOF)
                        chars_consumed++;
                    if ((next == L'x' || next == L'X')) {
                        base = 16;
                        if (i < MAX_SCAN_WIDTH)
                            buf[i++] = '0';
                        if (i < MAX_SCAN_WIDTH)
                            buf[i++] = (char)next;
                        if (width > 0)
                            width -= 2;
                        c = _in_wchar(stream);
                        if (c != WEOF)
                            chars_consumed++;
                    } else {
                        _unget_wchar(stream, next);
                        if (next != WEOF)
                            chars_consumed--;
                        if (base == 0)
                            base = 8;
                    }
                } else if (base == 0)
                    base = 8;
            } else if (base == 0)
                base = 10;

            while (c != WEOF) {
                if (width == 0) {
                    _unget_wchar(stream, c);
                    if (c != WEOF)
                        chars_consumed--;
                    break;
                }
                val = -1;

                /* Narrow the wchar to ASCII for base parsing */
                if (c < 128) {
                    if (isdigit((char)c))
                        val = c - L'0';
                    else if (isalpha((char)c))
                        val = tolower((char)c) - 'a' + 10;
                }

                if (val >= 0 && val < base) {
                    has_digit = 1;
                    if (i < MAX_SCAN_WIDTH)
                        buf[i++] = (char)c;
                    if (width > 0)
                        width--;
                    c = _in_wchar(stream);
                    if (c != WEOF)
                        chars_consumed++;
                } else {
                    _unget_wchar(stream, c);
                    if (c != WEOF)
                        chars_consumed--;
                    break;
                }
            }
            buf[i] = '\0';
            if (!has_digit)
                goto match_failure;

            if (!(flags & FL_SPLAT)) {
                if (conversion_char == L'p') {
                    *va_arg(arg, void **) = (void *)(uintptr_t)strtoull(buf, NULL, 16);
                } else {
                    unsigned long long res = strtoull(buf, NULL, base);
                    if (conversion_char == L'd' || conversion_char == L'i') {
                        long long sres = (long long)res;
                        if (flags & FL_LONGLONG)
                            *va_arg(arg, long long *) = sres;
                        else if (flags & FL_LONG)
                            *va_arg(arg, long *) = (long)sres;
                        else if (flags & FL_SHORT)
                            *va_arg(arg, short *) = (short)sres;
                        else if (flags & FL_SHORTSHORT)
                            *va_arg(arg, char *) = (char)sres;
                        else if (flags & FL_INTMAX)
                            *va_arg(arg, intmax_t *) = (intmax_t)sres;
                        else if (flags & FL_SIZE)
                            *va_arg(arg, ssize_t *) = (ssize_t)sres;
                        else if (flags & FL_PTRDIFF)
                            *va_arg(arg, ptrdiff_t *) = (ptrdiff_t)sres;
                        else
                            *va_arg(arg, int *) = (int)sres;
                    } else {
                        if (flags & FL_LONGLONG)
                            *va_arg(arg, unsigned long long *) = res;
                        else if (flags & FL_LONG)
                            *va_arg(arg, unsigned long *) = (unsigned long)res;
                        else if (flags & FL_SHORT)
                            *va_arg(arg, unsigned short *) = (unsigned short)res;
                        else if (flags & FL_SHORTSHORT)
                            *va_arg(arg, unsigned char *) = (unsigned char)res;
                        else if (flags & FL_INTMAX)
                            *va_arg(arg, uintmax_t *) = (uintmax_t)res;
                        else if (flags & FL_SIZE)
                            *va_arg(arg, size_t *) = (size_t)res;
                        else if (flags & FL_PTRDIFF)
                            *va_arg(arg, ptrdiff_t *) = (ptrdiff_t)res;
                        else
                            *va_arg(arg, unsigned int *) = (unsigned int)res;
                    }
                }
                nmatch++;
            }
            break;
        }

        case L'a':
        case L'A':
        case L'f':
        case L'F':
        case L'e':
        case L'E':
        case L'g':
        case L'G': {
            int is_hex_float = 0;
            i = 0;
            c = _in_wchar(stream);
            if (c != WEOF)
                chars_consumed++;
            if (c == WEOF)
                goto input_failure;

            if (c == L'-' || c == L'+') {
                buf[i++] = (char)c;
                c = _in_wchar(stream);
                if (c != WEOF)
                    chars_consumed++;
                if (width > 0)
                    width--;
            }

            digits = 0;
            while (c != WEOF && i < MAX_SCAN_WIDTH) {
                if (width == 0) {
                    _unget_wchar(stream, c);
                    if (c != WEOF)
                        chars_consumed--;
                    break;
                }

                int valid = 0;
                /* Safely enable hex-float parsing only if "0x" or "0X" is legitimately detected */
                if (c == L'x' || c == L'X') {
                    if (i > 0 && buf[i - 1] == '0' &&
                        (i == 1 || (i == 2 && (buf[0] == '+' || buf[0] == '-')))) {
                        valid = 1;
                        is_hex_float = 1;
                    }
                } else if (is_hex_float) {
                    if ((c < 128 && isxdigit((char)c)) || c == (wint_t)dp[0] || c == L'p' ||
                        c == L'P' || c == L'+' || c == L'-')
                        valid = 1;
                } else {
                    if ((c < 128 && isdigit((char)c)) || c == (wint_t)dp[0] || c == L'e' ||
                        c == L'E' || c == L'+' || c == L'-')
                        valid = 1;
                }

                if (valid) {
                    if (c < 128 && (is_hex_float ? isxdigit((char)c) : isdigit((char)c)))
                        digits = 1;
                    buf[i++] = (char)c;
                } else {
                    _unget_wchar(stream, c);
                    if (c != WEOF)
                        chars_consumed--;
                    break;
                }

                if (width > 0)
                    width--;
                c = _in_wchar(stream);
                if (c != WEOF)
                    chars_consumed++;
            }
            buf[i] = '\0';
            if (!digits)
                goto match_failure;

            if (!(flags & FL_SPLAT)) {
                if (flags & FL_LONGDOUBLE) {
                    *va_arg(arg, long double *) = strtold(buf, NULL);
                } else if (flags & FL_LONG) {
                    *va_arg(arg, double *) = strtod(buf, NULL);
                } else {
                    *va_arg(arg, float *) = strtof(buf, NULL);
                }
                nmatch++;
            }
            break;
        }

        case L'S':
        case L's': {
            int is_wide = (conversion_char == L'S' || (flags & FL_LONG));
            void *out_str = (flags & FL_SPLAT) ? NULL : va_arg(arg, void *);

            c = _skip_whitespace_wc(stream, &chars_consumed);
            if (c == WEOF)
                goto input_failure;

            _unget_wchar(stream, c);
            chars_consumed--;

            read_count = 0;
            if (width == -1)
                width = INT_MAX;

            while (width != 0) {
                c = _in_wchar(stream);
                if (c != WEOF)
                    chars_consumed++;

                if (c == WEOF || iswspace(c)) {
                    if (c != WEOF) {
                        _unget_wchar(stream, c);
                        chars_consumed--;
                    }
                    break;
                }

                if (out_str) {
                    if (is_wide) {
                        *(wchar_t *)out_str = (wchar_t)c;
                        out_str = (wchar_t *)out_str + 1;
                    } else {
                        char mbuf[MB_LEN_MAX];
                        mbstate_t mbs = {0};
                        size_t mlen = wcrtomb(mbuf, (wchar_t)c, &mbs);
                        if (mlen != (size_t)-1) {
                            for (size_t j = 0; j < mlen; j++) {
                                *(char *)out_str = mbuf[j];
                                out_str = (char *)out_str + 1;
                            }
                        }
                    }
                }
                read_count++;
                if (width > 0)
                    width--;
            }

            if (out_str) {
                if (is_wide)
                    *(wchar_t *)out_str = L'\0';
                else
                    *(char *)out_str = '\0';
            }
            if (read_count > 0 && !(flags & FL_SPLAT))
                nmatch++;
            else if (read_count == 0)
                goto input_failure;
            break;
        }

        case L'C':
        case L'c': {
            int is_wide = (conversion_char == L'C' || (flags & FL_LONG));
            void *out_str = (flags & FL_SPLAT) ? NULL : va_arg(arg, void *);
            if (width == -1)
                width = 1;

            read_count = 0;
            while (width-- > 0) {
                c = _in_wchar(stream);
                if (c != WEOF)
                    chars_consumed++;
                if (c == WEOF) {
                    if (read_count == 0 && nmatch == 0)
                        goto input_failure;
                    break;
                }
                if (out_str) {
                    if (is_wide) {
                        *(wchar_t *)out_str = (wchar_t)c;
                        out_str = (wchar_t *)out_str + 1;
                    } else {
                        char mbuf[MB_LEN_MAX];
                        mbstate_t mbs = {0};
                        size_t mlen = wcrtomb(mbuf, (wchar_t)c, &mbs);
                        if (mlen != (size_t)-1) {
                            for (size_t j = 0; j < mlen; j++) {
                                *(char *)out_str = mbuf[j];
                                out_str = (char *)out_str + 1;
                            }
                        }
                    }
                }
                read_count++;
            }
            if (!(flags & FL_SPLAT) && read_count > 0)
                nmatch++;
            break;
        }

        case L'[': {
            int is_wide = (flags & FL_LONG);
            void *out_str = (flags & FL_SPLAT) ? NULL : va_arg(arg, void *);
            int invert = 0;

            const wchar_t *set_start = p;
            if (*p == L'^') {
                invert = 1;
                p++;
                set_start++;
            }
            if (*p == L']')
                p++; // Special case: ] as first char
            while (*p && *p != L']')
                p++;
            const wchar_t *set_end = p;
            if (*p == L']')
                p++;

            c = _in_wchar(stream);
            if (c != WEOF)
                chars_consumed++;
            if (c == WEOF)
                goto input_failure;

            matched = 0;
            while (c != WEOF) {
                if (width == 0) {
                    _unget_wchar(stream, c);
                    if (c != WEOF)
                        chars_consumed--;
                    break;
                }

                m = _match_scanset(c, set_start, set_end);
                if (invert)
                    m = !m;
                if (!m) {
                    _unget_wchar(stream, c);
                    if (c != WEOF)
                        chars_consumed--;
                    break;
                }

                if (out_str) {
                    if (is_wide) {
                        *(wchar_t *)out_str = (wchar_t)c;
                        out_str = (wchar_t *)out_str + 1;
                    } else {
                        char mbuf[MB_LEN_MAX];
                        mbstate_t mbs = {0};
                        size_t mlen = wcrtomb(mbuf, (wchar_t)c, &mbs);
                        if (mlen != (size_t)-1) {
                            for (size_t j = 0; j < mlen; j++) {
                                *(char *)out_str = mbuf[j];
                                out_str = (char *)out_str + 1;
                            }
                        }
                    }
                }
                matched++;
                if (width > 0)
                    width--;
                c = _in_wchar(stream);
                if (c != WEOF)
                    chars_consumed++;
            }

            if (matched == 0)
                goto match_failure;
            if (out_str) {
                if (is_wide)
                    *(wchar_t *)out_str = L'\0';
                else
                    *(char *)out_str = '\0';
                nmatch++;
            }
            break;
        }
        default:
            break;
        }
    }

    if (!(stream->_flags & __S_STR))
        _spin_unlock(&stream->_lock);
    return nmatch;

match_failure:
    if (!(stream->_flags & __S_STR))
        _spin_unlock(&stream->_lock);
    return nmatch;

input_failure:
    if (!(stream->_flags & __S_STR))
        _spin_unlock(&stream->_lock);
    return (nmatch == 0) ? EOF : nmatch;
}
