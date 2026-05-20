#include <float.h> // Added for completeness, though raw limits are used
#include <limits.h>
#include <locale.h>
#include <math.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wchar.h>

#pragma clang diagnostic ignored "-Wunknown-warning-option"
#pragma clang diagnostic ignored "-Wreserved-identifier"

#define PAD_RIGHT 1
#define PAD_ZERO 2
#define PRINT_SGN 4
#define PRINT_SPC 8
#define PRINT_HEX_LO 16
#define PRINT_HEX_UP 32
#define PRINT_OCT 64
#define PRINT_ALT 128
#define PRINT_GROUP 256

#define MAX_PRECISION 256
#define MAX_WIDTH 256

/* Core output function: writes a character to the stream buffer or fd */
static int _out_char(FILE *f, int c)
{
    if (f->_flags & __S_STR) {
        if (f->_cnt > 0) {
            if (f->_base != NULL) {
                *f->_ptr++ = (unsigned char)c;
            }
            f->_cnt--;
        }
        return 1;
    }

    if (f->_flags & __S_NBF) {
        unsigned char ch = (unsigned char)c;
        if (write(f->_fd, &ch, 1) != 1) {
            return 0;
        }
        return 1;
    }

    if (f->_cnt > 0) {
        *f->_ptr++ = (unsigned char)c;
        f->_cnt--;
        f->_flags |= __S_DIRTY;
    } else {
        if (__stdio_flush_impl(f) == 0) {
            if (f->_cnt > 0) {
                *f->_ptr++ = (unsigned char)c;
                f->_cnt--;
                f->_flags |= __S_DIRTY;
            } else {
                unsigned char ch = (unsigned char)c;
                if (write(f->_fd, &ch, 1) != 1)
                    return 0;
            }
        } else {
            return 0;
        }
    }

    if ((f->_flags & __S_LBF) && (c == '\n')) {
        __stdio_flush_impl(f);
    }

    return 1;
}

/* Reverses a string in place (used for itoa algorithms) */
static void _reverse(char *s, int len)
{
    int i, j;
    char c;
    for (i = 0, j = len - 1; i < j; i++, j--) {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}

/* Converts unsigned integer to string with specified base */
static int _itoa_base(uintmax_t val, int base, int flags, char *buf)
{
    char *orig = buf;
    const char *digits = (flags & PRINT_HEX_UP) ? "0123456789ABCDEF" : "0123456789abcdef";

    if (val == 0) {
        *buf++ = '0';
        return 1;
    }

    while (val) {
        *buf++ = digits[val % (uintmax_t)base];
        val /= (uintmax_t)base;
    }
    return ((int)(buf - orig));
}

/* Converts unsigned integer to string with grouping separators */
static int _itoa_base_grouped(uintmax_t val,
                              int base,
                              int flags,
                              char *buf,
                              const char *sep,
                              const char *grp)
{
    char *orig = buf;
    const char *digits = (flags & PRINT_HEX_UP) ? "0123456789ABCDEF" : "0123456789abcdef";
    int count = 0;
    int sep_len = (int)strlen(sep);
    int current_grp = (grp && *grp) ? *grp : CHAR_MAX;

    if (val == 0) {
        *buf++ = '0';
        return 1;
    }

    while (val) {
        if (current_grp != CHAR_MAX && count == current_grp && count > 0) {
            for (int i = sep_len - 1; i >= 0; i--) {
                *buf++ = sep[i];
            }
            count = 0;
            if (*(grp + 1) != '\0') {
                grp++;
                current_grp = *grp;
            }
        }
        *buf++ = digits[val % (uintmax_t)base];
        val /= (uintmax_t)base;
        count++;
    }
    return ((int)(buf - orig));
}

/* Encodes a 32-bit wide character to UTF-8 */
static int _encode_utf8(uint32_t wc, char *buf)
{
    if (wc < 0x80) {
        buf[0] = (char)wc;
        return 1;
    } else if (wc < 0x800) {
        buf[0] = (char)(0xC0 | (wc >> 6));
        buf[1] = (char)(0x80 | (wc & 0x3F));
        return 2;
    } else if (wc < 0x10000) {
        buf[0] = (char)(0xE0 | (wc >> 12));
        buf[1] = (char)(0x80 | ((wc >> 6) & 0x3F));
        buf[2] = (char)(0x80 | (wc & 0x3F));
        return 3;
    } else if (wc < 0x110000) {
        buf[0] = (char)(0xF0 | (wc >> 18));
        buf[1] = (char)(0x80 | ((wc >> 12) & 0x3F));
        buf[2] = (char)(0x80 | ((wc >> 6) & 0x3F));
        buf[3] = (char)(0x80 | (wc & 0x3F));
        return 4;
    }
    return 0;
}

/* Formats standard floating point values (%f, %e, %g) */
static int _fmt_float(double val, int prec, int flags, char type, char *buf, const struct lconv *lc)
{
    double tmp, v, rounding, fpart;
    char *orig = buf, *num_start, *exp_start;
    int exp = 0, e, digit, elen;
    char fmt_type;
    uintmax_t ipart;
    const char *dp = (lc && lc->decimal_point && lc->decimal_point[0]) ? lc->decimal_point : ".";
    int dp_len = (int)strlen(dp);

    /* Handle NaN */
    if (val != val) {
        if (type >= 'a' && type <= 'z')
            strcpy(buf, "nan");
        else
            strcpy(buf, "NAN");
        return 3;
    }

    /* Handle Infinity */
    tmp = (val < 0) ? -val : val;
    if (tmp > 1.7976931348623157E+308) {
        if (type >= 'a' && type <= 'z')
            strcpy(buf, "inf");
        else
            strcpy(buf, "INF");
        return 3;
    }

    if (val < 0)
        val = -val;

    fmt_type = type;
    if (type == 'g' || type == 'G') {
        if (prec < 0)
            prec = 6;
        else if (prec == 0)
            prec = 1;

        v = val;
        e = 0;
        if (fabs(v) > 1e-300) {
            while (v >= 10.0) {
                v /= 10.0;
                e++;
            }
            while (v < 1.0) {
                v *= 10.0;
                e--;
            }
        }

        if (prec > e && e >= -4) {
            prec = prec - 1 - e;
            fmt_type = (type == 'g') ? 'f' : 'F';
        } else {
            prec = prec - 1;
            fmt_type = (type == 'g') ? 'e' : 'E';
        }
    }

    if (prec < 0)
        prec = 6;

    if (fmt_type == 'e' || fmt_type == 'E') {
        if (fabs(val) >= 1e-300) {
            while (val >= 10.0) {
                val /= 10.0;
                exp++;
            }
            while (val < 1.0) {
                val *= 10.0;
                exp--;
            }
        }
    }

    rounding = 0.5;
    for (int i = 0; i < prec; i++)
        rounding /= 10.0;
    val += rounding;

    if (fmt_type == 'e' || fmt_type == 'E') {
        if (val >= 10.0) {
            val /= 10.0;
            exp++;
        }
    }

    ipart = (uintmax_t)val;
    fpart = val - (double)ipart;

    num_start = buf;
    if ((flags & PRINT_GROUP) && lc && lc->thousands_sep[0]) {
        buf += _itoa_base_grouped(ipart, 10, 0, buf, lc->thousands_sep, lc->grouping);
    } else {
        buf += _itoa_base(ipart, 10, 0, buf);
    }

    _reverse(num_start, ((int)(buf - num_start)));

    if (prec > 0 || (flags & PRINT_ALT)) {
        strcpy(buf, dp);
        buf += dp_len;
    }

    if (prec > 0) {
        for (int i = 0; i < prec; i++) {
            fpart *= 10.0;
            digit = (int)fpart;
            *buf++ = '0' + ((char)(digit % 10));
            fpart -= digit;
        }
    }

    if ((type == 'g' || type == 'G') && !(flags & PRINT_ALT)) {
        char *end = buf - 1;
        if (strstr(num_start, dp)) {
            while (end > num_start && *end == '0') {
                end--;
                buf--;
            }
            if (buf - num_start >= dp_len && strncmp(buf - dp_len, dp, (unsigned)dp_len) == 0) {
                buf -= dp_len;
            }
        }
    }

    if (fmt_type == 'e' || fmt_type == 'E') {
        *buf++ = fmt_type;
        *buf++ = (exp >= 0) ? '+' : '-';
        if (exp < 0)
            exp = -exp;

        exp_start = buf;
        elen = _itoa_base((uintmax_t)exp, 10, 0, buf);
        buf += elen;
        if (elen < 2)
            *buf++ = '0';
        _reverse(exp_start, ((int)(buf - exp_start)));
    }

    return ((int)(buf - orig));
}

/* Fully implements C99 Hexadecimal Floating-Point formatting (%a, %A) */
static int _fmt_hex_float(double val,
                          int prec,
                          int flags,
                          char type,
                          char *buf,
                          const struct lconv *lc)
{
    char *orig = buf;
    const char *dp = (lc && lc->decimal_point && lc->decimal_point[0]) ? lc->decimal_point : ".";
    int dp_len = (int)strlen(dp);
    const char *hex_digits = (type == 'A') ? "0123456789ABCDEF" : "0123456789abcdef";
    int exp2 = 0;
    double m;

    /* Handle NaN and Infinity */
    if (val != val) {
        strcpy(buf, (type == 'A') ? "NAN" : "nan");
        return 3;
    }
    double tmp = (val < 0) ? -val : val;
    if (tmp > 1.7976931348623157E+308) {
        strcpy(buf, (type == 'A') ? "INF" : "inf");
        return 3;
    }

    if (val < 0)
        val = -val;

    /* Extract binary mantissa and exponent. frexp returns m in [0.5, 1.0) */
    m = frexp(val, &exp2);

    if (m == 0.0) {
        exp2 = 0;
        *buf++ = '0';
        /* Print padding zeros if precision is explicitly provided */
        int show_dp = (flags & PRINT_ALT) || (prec > 0);
        if (show_dp) {
            strcpy(buf, dp);
            buf += dp_len;
            for (int i = 0; i < (prec > 0 ? prec : 0); i++)
                *buf++ = '0';
        }
    } else {
        /* Normalize mantissa to [1.0, 2.0) range */
        m *= 2.0;
        exp2 -= 1;

        /* Apply hexadecimal rounding to nearest if precision is explicitly set */
        if (prec >= 0) {
            double rounding = 0.5;
            for (int i = 0; i < prec; i++)
                rounding /= 16.0;
            m += rounding;
            if (m >= 2.0) {
                m /= 2.0;
                exp2 += 1;
            }
        }

        /* Extract integer part (will always be 1 unless rounded up, handled above) */
        int digit = (int)m;
        *buf++ = hex_digits[digit];
        m -= digit;

        /* Determine if decimal point should be printed */
        int show_dp = (flags & PRINT_ALT) || (prec > 0) || (prec < 0 && m > 0.0);
        if (show_dp) {
            strcpy(buf, dp);
            buf += dp_len;
        }

        /* Extract fractional hexadecimal digits */
        if (prec > 0) {
            for (int i = 0; i < prec; i++) {
                m *= 16.0;
                digit = (int)m;
                *buf++ = hex_digits[digit];
                m -= digit;
            }
        } else if (prec < 0) {
            /* C99: If precision is omitted, write enough digits to represent exact value */
            while (m > 0.0 && (buf - orig) < 256) {
                m *= 16.0;
                digit = (int)m;
                *buf++ = hex_digits[digit];
                m -= digit;
            }
        }
    }

    /* Append exponent part (Base-2 power) */
    *buf++ = (type == 'A') ? 'P' : 'p';
    *buf++ = (exp2 >= 0) ? '+' : '-';
    if (exp2 < 0)
        exp2 = -exp2;

    char *exp_start = buf;
    int elen = _itoa_base((uintmax_t)exp2, 10, 0, buf);
    buf += elen;
    _reverse(exp_start, elen);

    return (int)(buf - orig);
}

int vfprintf(FILE *stream, const char *format, va_list arg)
{
    const char *p = format, *prefix;
    struct lconv *lc = localeconv();
    int total_written = 0, flags, width, prec, len_mod, slen, base, is_signed, is_integer, is_float;
    int total_bytes, padding, written_so_far, prec_zeros, pad_len, prefix_len, sgn_len, actual_len;
    char temp_buf[512], type, *str_val, dummy[4], sgn_char;
    uintmax_t uval;
    intmax_t sval;
    double fval;
    uint32_t *ptr;

    int mode = fwide(stream, 0);
    if (mode > 0)
        return EOF;
    else if (mode == 0)
        fwide(stream, -1);

    if (!stream)
        return -1;

    if (!(stream->_flags & __S_STR)) {
        _spin_lock(&stream->_lock);
    }

    if (stream->_flags & __S_RD) {
        stream->_flags &= ~__S_RD;
        stream->_flags |= __S_WR;
        stream->_cnt = stream->_bsize;
        stream->_ptr = stream->_base;
    } else if (!(stream->_flags & __S_WR)) {
        stream->_flags |= __S_WR;
        if (stream->_cnt == 0 && stream->_ptr == stream->_base) {
            stream->_cnt = stream->_bsize;
        }
    }

    while (*p) {
        if (*p != '%') {
            total_written += _out_char(stream, *p++);
            continue;
        }

        p++;

        flags = 0;
        width = 0;
        prec = -1;
        len_mod = 0;

        /* Parse flags */
        while (1) {
            if (*p == '-')
                flags |= PAD_RIGHT;
            else if (*p == '+')
                flags |= PRINT_SGN;
            else if (*p == ' ')
                flags |= PRINT_SPC;
            else if (*p == '#')
                flags |= PRINT_ALT;
            else if (*p == '0')
                flags |= PAD_ZERO;
            else if (*p == '\'')
                flags |= PRINT_GROUP;
            else
                break;
            p++;
        }

        /* Parse width */
        if (*p == '*') {
            width = va_arg(arg, int);
            if (width < 0) {
                flags |= PAD_RIGHT;
                width = -width;
            }
            p++;
        } else {
            while (*p >= '0' && *p <= '9') {
                width = width * 10 + (*p - '0');
                p++;
            }
        }

        if (width > MAX_WIDTH)
            width = MAX_WIDTH;

        /* Parse precision */
        if (*p == '.') {
            p++;
            if (*p == '*') {
                prec = va_arg(arg, int);
                if (prec < 0)
                    prec = -1;
                p++;
            } else {
                prec = 0;
                while (*p >= '0' && *p <= '9') {
                    prec = prec * 10 + (*p - '0');
                    p++;
                }
            }
        }

        if (prec > MAX_PRECISION)
            prec = MAX_PRECISION;

        /* Parse length modifiers including C99 'j' */
        if (*p == 'h') {
            len_mod = 1;
            p++;
            if (*p == 'h') {
                len_mod = 2;
                p++;
            }
        } else if (*p == 'l') {
            len_mod = 3;
            p++;
            if (*p == 'l') {
                len_mod = 4;
                p++;
            }
        } else if (*p == 'L') {
            len_mod = 5; /* long double */
            p++;
        } else if (*p == 'z') {
            len_mod = 6;
            p++;
        } else if (*p == 't') {
            len_mod = 7;
            p++;
        } else if (*p == 'j') {
            len_mod = 8; /* intmax_t / uintmax_t */
            p++;
        }

        type = *p++;
        str_val = NULL;
        slen = 0;

        uval = 0;
        sval = 0;
        fval = 0.0;

        base = 10;
        is_signed = 0;
        is_integer = 0;
        is_float = 0;

        switch (type) {
        case 'd':
        case 'i':
            is_signed = 1;
            is_integer = 1;
            if (len_mod == 4)
                sval = va_arg(arg, long long);
            else if (len_mod == 3)
                sval = va_arg(arg, long);
            else if (len_mod == 6 || len_mod == 7)
                sval = va_arg(arg, ssize_t);
            else if (len_mod == 8)
                sval = va_arg(arg, intmax_t);
            else if (len_mod == 1)
                sval = (short)va_arg(arg, int);
            else if (len_mod == 2)
                sval = (char)va_arg(arg, int);
            else
                sval = va_arg(arg, int);

            uval = (uintmax_t)llabs(sval);
            break;

        case 'u':
        case 'o':
        case 'x':
        case 'X':
        case 'b':
            is_integer = 1;
            if (type == 'o') {
                base = 8;
                flags |= PRINT_OCT;
            } else if (type == 'x') {
                base = 16;
                flags |= PRINT_HEX_LO;
            } else if (type == 'X') {
                base = 16;
                flags |= PRINT_HEX_UP;
            } else if (type == 'b') {
                base = 2;
            }

            if (len_mod == 4)
                uval = va_arg(arg, unsigned long long);
            else if (len_mod == 3)
                uval = va_arg(arg, unsigned long);
            else if (len_mod == 6)
                uval = va_arg(arg, size_t);
            else if (len_mod == 7)
                uval = va_arg(arg, uintptr_t);
            else if (len_mod == 8)
                uval = va_arg(arg, uintmax_t);
            else if (len_mod == 1)
                uval = (unsigned short)va_arg(arg, unsigned int);
            else if (len_mod == 2)
                uval = (unsigned char)va_arg(arg, unsigned int);
            else
                uval = va_arg(arg, unsigned int);
            break;

        case 'p':
            uval = (uintptr_t)va_arg(arg, void *);
            base = 16;
            flags |= (PRINT_HEX_LO | PRINT_ALT);
            is_integer = 1;
            break;

        case 'n': {
            ptr = va_arg(arg, uint32_t *);
            if (ptr)
                *ptr = (unsigned)total_written;
            continue;
        }

        case 'C':
        case 'c':
            if (type == 'C' || len_mod == 3) {
                uint32_t wc = (uint32_t)va_arg(arg, unsigned int);
                slen = _encode_utf8(wc, temp_buf);
                str_val = temp_buf;
            } else {
                temp_buf[0] = (char)va_arg(arg, int);
                slen = 1;
                str_val = temp_buf;
            }
            break;

        case 'S':
        case 's':
            if (type == 'S' || len_mod == 3) {
                uint32_t *wstr = va_arg(arg, uint32_t *);
                if (!wstr)
                    wstr = (uint32_t *)L"(null)";

                total_bytes = 0;
                ptr = wstr;

                while (*ptr) {
                    int char_len = _encode_utf8((uint32_t)*ptr, dummy);
                    if (prec >= 0 && (total_bytes + char_len) > prec)
                        break;
                    total_bytes += char_len;
                    ptr++;
                }

                padding = width - total_bytes;
                if (!(flags & PAD_RIGHT)) {
                    while (padding-- > 0)
                        total_written += _out_char(stream, ' ');
                }

                written_so_far = 0;
                ptr = wstr;
                while (*ptr) {
                    char utf8_buf[4];
                    int char_len = _encode_utf8((uint32_t)*ptr, utf8_buf);

                    if (prec >= 0 && (written_so_far + char_len) > prec)
                        break;

                    for (int i = 0; i < char_len; i++)
                        total_written += _out_char(stream, utf8_buf[i]);
                    written_so_far += char_len;
                    ptr++;
                }

                if (flags & PAD_RIGHT) {
                    while (padding-- > 0)
                        total_written += _out_char(stream, ' ');
                }
                continue;
            } else {
                str_val = va_arg(arg, char *);
                if (!str_val)
                    str_val = "(null)";
                slen = (int)strlen(str_val);
                if (prec >= 0 && slen > prec)
                    slen = prec;
            }
            break;

        case 'a':
        case 'A':
            is_float = 1;
            /* Cast long double to double to maintain pure math compatibility if L is passed */
            fval = (len_mod == 5) ? (double)va_arg(arg, long double) : va_arg(arg, double);
            if (fval < 0)
                flags |= PRINT_SGN;
            slen = _fmt_hex_float(fval, prec, flags, type, temp_buf, lc);
            str_val = temp_buf;
            break;

        case 'f':
        case 'F':
        case 'e':
        case 'E':
        case 'g':
        case 'G':
            is_float = 1;
            fval = (len_mod == 5) ? (double)va_arg(arg, long double) : va_arg(arg, double);
            if (fval < 0) {
                flags |= PRINT_SGN;
            }
            slen = _fmt_float(fval, prec, flags, type, temp_buf, lc);
            str_val = temp_buf;
            break;

        case '%':
            temp_buf[0] = '%';
            slen = 1;
            str_val = temp_buf;
            break;

        default:
            temp_buf[0] = type;
            slen = 1;
            str_val = temp_buf;
            break;
        }

        if (is_integer) {
            if (prec >= 0)
                flags &= ~PAD_ZERO;

            if ((flags & PRINT_GROUP) && base == 10 && lc->thousands_sep && lc->thousands_sep[0]) {
                slen = _itoa_base_grouped(
                    uval, base, flags, temp_buf, lc->thousands_sep, lc->grouping);
            } else {
                slen = _itoa_base(uval, base, flags, temp_buf);
            }

            _reverse(temp_buf, slen);
            str_val = temp_buf;
        }

        prec_zeros = 0;
        if (is_integer && prec >= 0) {
            if (prec > slen)
                prec_zeros = prec - slen;
            if (prec == 0 && uval == 0 && !(flags & PRINT_ALT && base == 8)) {
                slen = 0;
            }
        }

        pad_len = 0;
        sgn_char = 0;
        if (is_signed || is_float) {
            if ((is_signed && sval < 0) || (is_float && fval < 0))
                sgn_char = '-';
            else if (flags & PRINT_SGN)
                sgn_char = '+';
            else if (flags & PRINT_SPC)
                sgn_char = ' ';
        }

        prefix = "";
        prefix_len = 0;

        /* Force 0x prefix for hexadecimal floats unless it evaluates to NaN/Inf */
        if (type == 'a' || type == 'A') {
            if (str_val[0] == '0' || str_val[0] == '1') {
                prefix = (type == 'A') ? "0X" : "0x";
                prefix_len = 2;
            }
        } else if ((flags & PRINT_ALT) && (uval != 0 || type == 'p')) {
            if (base == 8) {
                prefix = "0";
                prefix_len = 1;
            } else if (base == 16) {
                prefix = (flags & PRINT_HEX_UP) ? "0X" : "0x";
                prefix_len = 2;
            }
        }
        if (base == 8 && (flags & PRINT_ALT) && (slen + prec_zeros) > 0 && str_val[0] == '0' &&
            prec_zeros == 0) {
            prefix = "";
            prefix_len = 0;
        }

        sgn_len = (sgn_char != 0);
        actual_len = sgn_len + prefix_len + prec_zeros + slen;
        pad_len = width - actual_len;

        if (!(flags & PAD_RIGHT) && !(flags & PAD_ZERO)) {
            while (pad_len-- > 0)
                total_written += _out_char(stream, ' ');
        }

        if (sgn_char)
            total_written += _out_char(stream, sgn_char);

        for (int k = 0; k < prefix_len; k++)
            total_written += _out_char(stream, prefix[k]);

        if (!(flags & PAD_RIGHT) && (flags & PAD_ZERO)) {
            while (pad_len-- > 0)
                total_written += _out_char(stream, '0');
        }

        while (prec_zeros-- > 0)
            total_written += _out_char(stream, '0');

        for (int i = 0; i < slen; i++)
            total_written += _out_char(stream, str_val[i]);

        if (flags & PAD_RIGHT) {
            while (pad_len-- > 0)
                total_written += _out_char(stream, ' ');
        }
    }

    if (!(stream->_flags & __S_STR)) {
        _spin_unlock(&stream->_lock);
    }

    return total_written;
}
