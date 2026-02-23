#include <math.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PAD_RIGHT 1
#define PAD_ZERO 2
#define PRINT_SGN 4
#define PRINT_SPC 8
#define PRINT_HEX_LO 16
#define PRINT_HEX_UP 32
#define PRINT_OCT 64
#define PRINT_ALT 128

#define MAX_PRECISION 256
#define MAX_WIDTH 256

static int _out_char(FILE *f, int c) {
    /* Optimization for sprintf (String buffer) - no locking, no flushing logic needed */
    if(f->_flags & __S_STR) {
        if(f->_cnt > 0) {
            /* Check if base is not NULL before writing. */
            if(f->_base != NULL) {
                *f->_ptr++ = (unsigned char)c;
            }
            f->_cnt--;
        }
        return 1;
    }

    /* Handle Unbuffered I/O (_IONBF) directly to avoid NULL pointer dereference */
    if(f->_flags & __S_NBF) {
        unsigned char ch = (unsigned char)c;
        if(write(f->_fd, &ch, 1) != 1) {
            return 0; /* Error */
        }
        return 1;
    }

    /* Regular buffered stream logic */
    if(f->_cnt > 0) {
        *f->_ptr++ = (unsigned char)c;
        f->_cnt--;
        f->_flags |= __S_DIRTY;
    } else {
        /* Buffer full, flush using internal impl */
        if(__stdio_flush_impl(f) == 0) {
            /* After flush, pointers should be reset. Double check we have space now. */
            /* If flush succeeded but we still have no buffer (unlikely if not NBF), write directly */
            if(f->_cnt > 0) {
                *f->_ptr++ = (unsigned char)c;
                f->_cnt--;
                f->_flags |= __S_DIRTY;
            } else {
                unsigned char ch = (unsigned char)c;
                if(write(f->_fd, &ch, 1) != 1)
                    return 0;
            }
        } else {
            return 0; /* Error */
        }
    }

    /* Support Line Buffering (_IOLBF) */
    if((f->_flags & __S_LBF) && (c == '\n')) {
        __stdio_flush_impl(f);
    }

    return 1;
}

static void _reverse(char *s, int len) {
    int i, j;
    char c;
    for(i = 0, j = len - 1; i < j; i++, j--) {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}

static int _itoa_base(uintmax_t val, int base, int flags, char *buf) {
    char *orig = buf;
    const char *digits = (flags & PRINT_HEX_UP) ? "0123456789ABCDEF" : "0123456789abcdef";

    if(val == 0) {
        *buf++ = '0';
        return 1;
    }

    while(val) {
        *buf++ = digits[val % (uintmax_t)base];
        val /= (uintmax_t)base;
    }
    return buf - orig;
}

/* 
 * Helper to encode a single wide char to UTF-8.
 * Returns the number of bytes written to buf (0 on failure).
 * Ensure buf has at least 4 bytes of space.
 */
static int _encode_utf8(uint32_t wc, char *buf) {
    if(wc < 0x80) {
        buf[0] = (char)wc;
        return 1;
    } else if(wc < 0x800) {
        buf[0] = (char)(0xC0 | (wc >> 6));
        buf[1] = (char)(0x80 | (wc & 0x3F));
        return 2;
    } else if(wc < 0x10000) {
        buf[0] = (char)(0xE0 | (wc >> 12));
        buf[1] = (char)(0x80 | ((wc >> 6) & 0x3F));
        buf[2] = (char)(0x80 | (wc & 0x3F));
        return 3;
    } else if(wc < 0x110000) {
        buf[0] = (char)(0xF0 | (wc >> 18));
        buf[1] = (char)(0x80 | ((wc >> 12) & 0x3F));
        buf[2] = (char)(0x80 | ((wc >> 6) & 0x3F));
        buf[3] = (char)(0x80 | (wc & 0x3F));
        return 4;
    }
    return 0;
}

static int _fmt_float(double val, int prec, int flags, char type, char *buf) {
    char *orig = buf;
    int exp = 0;

    if(val != val) {
        if(type >= 'a' && type <= 'z')
            strcpy(buf, "nan");
        else
            strcpy(buf, "NAN");
        return 3;
    }

    double tmp = (val < 0) ? -val : val;
    if(tmp > 1.7976931348623157E+308) {
        if(type >= 'a' && type <= 'z')
            strcpy(buf, "inf");
        else
            strcpy(buf, "INF");
        return 3;
    }

    if(val < 0)
        val = -val;

    char fmt_type = type;
    if(type == 'g' || type == 'G') {
        if(prec < 0)
            prec = 6;
        else if(prec == 0)
            prec = 1;

        double v = val;
        int e = 0;
        if(fabs(v) >= 1e-15) {
            while(v >= 10.0) {
                v /= 10.0;
                e++;
            }
            while(v < 1.0) {
                v *= 10.0;
                e--;
            }
        }

        if(prec > e && e >= -4) {
            prec = prec - 1 - e;
            fmt_type = (type == 'g') ? 'f' : 'F';
        } else {
            prec = prec - 1;
            fmt_type = (type == 'g') ? 'e' : 'E';
        }
    }

    if(prec < 0)
        prec = 6;

    if(fmt_type == 'e' || fmt_type == 'E') {
        if(fabs(val) >= 1e-15) {
            while(val >= 10.0) {
                val /= 10.0;
                exp++;
            }
            while(val < 1.0) {
                val *= 10.0;
                exp--;
            }
        }
    }

    double rounding = 0.5;
    for(int i = 0; i < prec; i++)
        rounding /= 10.0;
    val += rounding;

    if(fmt_type == 'e' || fmt_type == 'E') {
        if(val >= 10.0) {
            val /= 10.0;
            exp++;
        }
    }

    uintmax_t ipart = (uintmax_t)val;
    double fpart = val - (double)ipart;

    char *num_start = buf;
    buf += _itoa_base(ipart, 10, 0, buf);

    /* Fixed incorrect mirroring: reverse integer part immediately so it is forward-oriented, matching the fractional part. */
    _reverse(num_start, buf - num_start);

    if(prec > 0 || (flags & PRINT_ALT)) {
        *buf++ = '.';
    }

    if(prec > 0) {
        for(int i = 0; i < prec; i++) {
            fpart *= 10.0;
            int digit = (int)fpart;
            *buf++ = '0' + (digit % 10);
            fpart -= digit;
        }
    }

    if((type == 'g' || type == 'G') && !(flags & PRINT_ALT)) {
        char *end = buf - 1;
        if(strchr(num_start, '.')) {
            while(end > num_start && *end == '0') {
                end--;
                buf--;
            }
            if(end > num_start && *end == '.') {
                buf--;
            }
        }
    }

    if(fmt_type == 'e' || fmt_type == 'E') {
        /* No global reverse needed here anymore */
        *buf++ = fmt_type;
        *buf++ = (exp >= 0) ? '+' : '-';
        if(exp < 0)
            exp = -exp;

        char *exp_start = buf;
        int elen = _itoa_base((uintmax_t)exp, 10, 0, buf);
        if(elen < 2)
            *buf++ = '0';
        _reverse(exp_start, buf - exp_start);
    } else {
        /* No global reverse needed here anymore */
    }

    return buf - orig;
}

int vfprintf(FILE *stream, const char *format, va_list arg) {
    const char *p = format;
    int total_written = 0;
    char temp_buf[512];

    if(!stream)
        return -1;

    if(!(stream->_flags & __S_STR)) {
        _spin_lock(&stream->_lock);
    }

    if(stream->_flags & __S_RD) {
        /* Transition from Read to Write */
        stream->_flags &= ~__S_RD;
        stream->_flags |= __S_WR;
        /* Invalidate read buffer */
        stream->_cnt = stream->_bsize;
        stream->_ptr = stream->_base;
    } else if(!(stream->_flags & __S_WR)) {
        /* Transition from Neutral to Write */
        stream->_flags |= __S_WR;
        /* Initialize buffer pointers if they were cleared */
        if(stream->_cnt == 0 && stream->_ptr == stream->_base) {
            stream->_cnt = stream->_bsize;
        }
    }

    while(*p) {
        if(*p != '%') {
            total_written += _out_char(stream, *p++);
            continue;
        }

        p++;

        int flags = 0;
        int width = 0;
        int prec = -1;
        int len_mod = 0;

        while(1) {
            if(*p == '-')
                flags |= PAD_RIGHT;
            else if(*p == '+')
                flags |= PRINT_SGN;
            else if(*p == ' ')
                flags |= PRINT_SPC;
            else if(*p == '#')
                flags |= PRINT_ALT;
            else if(*p == '0')
                flags |= PAD_ZERO;
            else
                break;
            p++;
        }

        if(*p == '*') {
            width = va_arg(arg, int);
            if(width < 0) {
                flags |= PAD_RIGHT;
                width = -width;
            }
            p++;
        } else {
            while(*p >= '0' && *p <= '9') {
                width = width * 10 + (*p - '0');
                p++;
            }
        }

        if(width > MAX_WIDTH)
            width = MAX_WIDTH;

        if(*p == '.') {
            p++;
            if(*p == '*') {
                prec = va_arg(arg, int);
                if(prec < 0)
                    prec = -1;
                p++;
            } else {
                prec = 0;
                while(*p >= '0' && *p <= '9') {
                    prec = prec * 10 + (*p - '0');
                    p++;
                }
            }
        }

        if(prec > MAX_PRECISION)
            prec = MAX_PRECISION;

        if(*p == 'h') {
            len_mod = 1;
            p++;
            if(*p == 'h') {
                len_mod = 2;
                p++;
            }
        } else if(*p == 'l') {
            len_mod = 3;
            p++;
            if(*p == 'l') {
                len_mod = 4;
                p++;
            }
        } else if(*p == 'L') {
            len_mod = 5;
            p++;
        } else if(*p == 'z') {
            len_mod = 6;
            p++;
        } else if(*p == 't') {
            len_mod = 7;
            p++;
        }

        char type = *p++;
        char *str_val = NULL;
        int slen = 0;

        uintmax_t uval = 0;
        intmax_t sval = 0;
        double fval = 0.0;

        int base = 10;
        int is_signed = 0;
        int is_integer = 0;
        int is_float = 0;

        switch(type) {
        case 'd':
        case 'i':
            is_signed = 1;
            is_integer = 1;
            if(len_mod == 4)
                sval = va_arg(arg, long long);
            else if(len_mod == 3)
                sval = va_arg(arg, long);
            else if(len_mod == 6 || len_mod == 7)
                sval = va_arg(arg, ssize_t);
            else if(len_mod == 1)
                sval = (short)va_arg(arg, int);
            else if(len_mod == 2)
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
            if(type == 'o') {
                base = 8;
                flags |= PRINT_OCT;
            } else if(type == 'x') {
                base = 16;
                flags |= PRINT_HEX_LO;
            } else if(type == 'X') {
                base = 16;
                flags |= PRINT_HEX_UP;
            } else if(type == 'b') {
                base = 2;
            }

            if(len_mod == 4)
                uval = va_arg(arg, unsigned long long);
            else if(len_mod == 3)
                uval = va_arg(arg, unsigned long);
            else if(len_mod == 6)
                uval = va_arg(arg, size_t);
            else if(len_mod == 7)
                uval = va_arg(arg, uintptr_t);
            else if(len_mod == 1)
                uval = (unsigned short)va_arg(arg, unsigned int);
            else if(len_mod == 2)
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
            int *ptr = va_arg(arg, int *);
            if(ptr)
                *ptr = total_written;
            continue;
        }

        case 'C':
        case 'c':
            /* Handle %lc / %lC (Wide Char) */
            if(type == 'C' || len_mod == 3) {
                /* wint_t is usually passed as int-sized arg */
                uint32_t wc = (uint32_t)va_arg(arg, unsigned int);
                slen = _encode_utf8(wc, temp_buf);
                str_val = temp_buf;
            } else {
                /* Standard Char */
                temp_buf[0] = (char)va_arg(arg, int);
                slen = 1;
                str_val = temp_buf;
            }
            break;

        case 'S':
        case 's':
            /* Handle %ls / %lS (Wide String to UTF-8) */
            if(type == 'S' || len_mod == 3) {
                wchar_t *wstr = va_arg(arg, wchar_t *);
                if(!wstr)
                    wstr = (wchar_t *)L"(null)";

                /* Calculate length in bytes for UTF-8 output, respecting precision */
                int total_bytes = 0;
                const wchar_t *ptr = wstr;
                char dummy[4];

                while(*ptr) {
                    int char_len = _encode_utf8((uint32_t)*ptr, dummy);
                    if(prec >= 0 && (total_bytes + char_len) > prec)
                        break;
                    total_bytes += char_len;
                    ptr++;
                }

                /* Handle Left Padding */
                int padding = width - total_bytes;
                if(!(flags & PAD_RIGHT)) {
                    while(padding-- > 0)
                        total_written += _out_char(stream, ' ');
                }

                /* Output the encoded characters */
                int written_so_far = 0;
                ptr = wstr;
                while(*ptr) {
                    char utf8_buf[4];
                    int char_len = _encode_utf8((uint32_t)*ptr, utf8_buf);

                    /* Stop if next char exceeds precision */
                    if(prec >= 0 && (written_so_far + char_len) > prec)
                        break;

                    for(int i = 0; i < char_len; i++)
                        total_written += _out_char(stream, utf8_buf[i]);
                    written_so_far += char_len;
                    ptr++;
                }

                /* Handle Right Padding */
                if(flags & PAD_RIGHT) {
                    while(padding-- > 0)
                        total_written += _out_char(stream, ' ');
                }

                /* Skip the generic footer loop since we handled output here */
                continue;
            } else {
                /* Standard String */
                str_val = va_arg(arg, char *);
                if(!str_val)
                    str_val = "(null)";
                slen = (int)strlen(str_val);
                if(prec >= 0 && slen > prec)
                    slen = prec;
            }
            break;

        case 'f':
        case 'F':
        case 'e':
        case 'E':
        case 'g':
        case 'G':
            is_float = 1;
            fval = va_arg(arg, double);
            if(fval < 0) {
                flags |= PRINT_SGN;
            }
            slen = _fmt_float(fval, prec, flags, type, temp_buf);
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

        if(is_integer) {
            if(prec >= 0)
                flags &= ~PAD_ZERO;
            slen = _itoa_base(uval, base, flags, temp_buf);
            _reverse(temp_buf, slen);
            str_val = temp_buf;
        }

        int prec_zeros = 0;
        if(is_integer && prec >= 0) {
            if(prec > slen)
                prec_zeros = prec - slen;
            if(prec == 0 && uval == 0 && !(flags & PRINT_ALT && base == 8)) {
                slen = 0;
            }
        }

        int pad_len = 0;
        char sgn_char = 0;
        if(is_signed || is_float) {
            if((is_signed && sval < 0) || (is_float && fval < 0))
                sgn_char = '-';
            else if(flags & PRINT_SGN)
                sgn_char = '+';
            else if(flags & PRINT_SPC)
                sgn_char = ' ';
        }

        const char *prefix = "";
        int prefix_len = 0;
        if((flags & PRINT_ALT) && (uval != 0 || type == 'p')) {
            if(base == 8) {
                prefix = "0";
                prefix_len = 1;
            } else if(base == 16) {
                prefix = (flags & PRINT_HEX_UP) ? "0X" : "0x";
                prefix_len = 2;
            }
        }
        if(base == 8 && (flags & PRINT_ALT) && (slen + prec_zeros) > 0 && str_val[0] == '0' && prec_zeros == 0) {
            prefix = "";
            prefix_len = 0;
        }

        int sgn_len = (sgn_char != 0);
        int actual_len = sgn_len + prefix_len + prec_zeros + slen;
        pad_len = width - actual_len;

        if(!(flags & PAD_RIGHT) && !(flags & PAD_ZERO)) {
            while(pad_len-- > 0)
                total_written += _out_char(stream, ' ');
        }

        if(sgn_char)
            total_written += _out_char(stream, sgn_char);

        for(int k = 0; k < prefix_len; k++)
            total_written += _out_char(stream, prefix[k]);

        if(!(flags & PAD_RIGHT) && (flags & PAD_ZERO)) {
            while(pad_len-- > 0)
                total_written += _out_char(stream, '0');
        }

        while(prec_zeros-- > 0)
            total_written += _out_char(stream, '0');

        for(int i = 0; i < slen; i++)
            total_written += _out_char(stream, str_val[i]);

        if(flags & PAD_RIGHT) {
            while(pad_len-- > 0)
                total_written += _out_char(stream, ' ');
        }
    }

    if(!(stream->_flags & __S_STR)) {
        _spin_unlock(&stream->_lock);
    }

    return total_written;
}
