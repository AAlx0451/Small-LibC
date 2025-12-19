/*
 * implementation of vfscanf compliant with ANSI C (C89/C90) and POSIX.
 */

#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stddef.h>

/* Internal limits */
#define MAX_SCAN_WIDTH 256

/*
 * Helper: _in_char
 * Reads a character from the stream, handling buffering and locking requirements.
 * Assumes caller holds stream->_lock.
 */
static int _in_char(FILE *f) {
    /* Handle String Streams (sscanf) */
    if (f->_flags & __S_STR) {
        if (f->_cnt > 0) {
            f->_cnt--;
            return (unsigned char)(*f->_ptr++);
        }
        return EOF;
    }

    /* Handle File Streams */
    if (f->_cnt > 0) {
        f->_cnt--;
        return (unsigned char)(*f->_ptr++);
    }

    /* Buffer empty, attempt refill */
    if (__stdio_fill_impl(f) == 0) {
        if (f->_cnt > 0) {
            f->_cnt--;
            return (unsigned char)(*f->_ptr++);
        }
    }

    return EOF;
}

/*
 * Helper: _unget_char
 * Pushes back a character to the stream.
 * Assumes caller holds stream->_lock.
 */
static void _unget_char(FILE *f, int c) {
    if (c == EOF) return;

    /* String stream */
    if (f->_flags & __S_STR) {
        if (f->_ptr > f->_base) {
            f->_ptr--;
            f->_cnt++;
        }
        return;
    }

    /* File stream */
    f->_ptr--;
    f->_cnt++;
    *f->_ptr = (unsigned char)c;
    f->_flags &= ~__S_EOF;
}

/* 
 * Helper: skip_whitespace
 */
static int _skip_whitespace(FILE *f, int *consumed) {
    int c;
    while ((c = _in_char(f)) != EOF) {
        (*consumed)++;
        if (!isspace(c)) return c;
    }
    return EOF;
}

/* 
 * Flag definitions for internal parser state 
 */
#define FL_SPLAT    0x01
#define FL_WIDTH    0x02
#define FL_LONG     0x04
#define FL_LONGLONG 0x08
#define FL_SHORT    0x10
#define FL_SHORTSHORT 0x20
#define FL_DOUBLE   0x40

int vfscanf(FILE *stream, const char *format, va_list arg) {
    const char *p = format;
    int c;
    int nmatch = 0;
    int width;
    int flags;
    int conversion_char;
    char buf[MAX_SCAN_WIDTH + 1]; 
    int i;
    int chars_consumed = 0; // For %n specifier

    if (!stream || !format) return EOF;

    /* LOCKING */
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
            stream->_cnt = 0;
            stream->_ptr = stream->_base;
        }
    }

    while (*p) {
        if (isspace((unsigned char)*p)) {
            while (isspace((unsigned char)*p)) p++;
            c = _skip_whitespace(stream, &chars_consumed);
            _unget_char(stream, c);
            if (c != EOF) chars_consumed--; // Correct for unget
            continue;
        }

        if (*p != '%') {
            c = _in_char(stream);
            if (c != EOF) chars_consumed++;
            if (c == EOF) goto input_failure;
            if (c != (unsigned char)*p) {
                _unget_char(stream, c);
                if (c != EOF) chars_consumed--;
                goto match_failure;
            }
            p++;
            continue;
        }

        p++; 
        if (*p == '%') {
            c = _in_char(stream);
            if (c != EOF) chars_consumed++;
            if (c == EOF) goto input_failure;
            if (c != '%') {
                _unget_char(stream, c);
                if (c != EOF) chars_consumed--;
                goto match_failure;
            }
            p++;
            continue;
        }

        flags = 0;
        width = 0;

        if (*p == '*') { flags |= FL_SPLAT; p++; }

        if (isdigit((unsigned char)*p)) {
            flags |= FL_WIDTH;
            while (isdigit((unsigned char)*p)) {
                width = width * 10 + (*p - '0');
                p++;
            }
            if (width == 0) width = 1;
        } else {
            width = -1;
        }

        if (*p == 'h') {
            p++;
            if (*p == 'h') { flags |= FL_SHORTSHORT; p++; }
            else { flags |= FL_SHORT; }
        } else if (*p == 'l') {
            p++;
            if (*p == 'l') { flags |= FL_LONGLONG; p++; }
            else { flags |= FL_LONG; }
        } else if (*p == 'L') {
            flags |= FL_DOUBLE; p++;
        } else if (*p == 'z' || *p == 't' || *p == 'j') {
             flags |= FL_LONG; p++; 
        }

        conversion_char = *p++;

        if (conversion_char != 'c' && conversion_char != '[' && conversion_char != 'n') {
            c = _skip_whitespace(stream, &chars_consumed);
            if (c == EOF) goto input_failure;
            _unget_char(stream, c);
            if (c != EOF) chars_consumed--; // Correct for unget
        }

        switch (conversion_char) {
            case 'n':
                if (!(flags & FL_SPLAT)) {
                    *va_arg(arg, int*) = chars_consumed;
                }
                continue; // %n does not perform a conversion, continue main loop

            case 'd': case 'i': case 'u': case 'x': case 'X': case 'o': case 'p': {
                int base = 10;
                int is_neg = 0;
                int has_digit = 0;
                
                if (conversion_char == 'i') base = 0;
                else if (conversion_char == 'x' || conversion_char == 'X' || conversion_char == 'p') base = 16;
                else if (conversion_char == 'o') base = 8;
                
                c = _in_char(stream);
                if (c != EOF) chars_consumed++;
                if (c == EOF) goto input_failure;

                if (c == '-') { is_neg = 1; c = _in_char(stream); if(c!=EOF) chars_consumed++; if(width>0) width--; }
                else if (c == '+') { c = _in_char(stream); if(c!=EOF) chars_consumed++; if(width>0) width--; }
                
                i = 0;
                if (is_neg) buf[i++] = '-';

                if (c == '0') {
                    has_digit = 1;
                    if (width != 0 && (base == 0 || base == 16)) {
                         int next = _in_char(stream);
                         if(next!=EOF) chars_consumed++;
                         if ((next == 'x' || next == 'X')) {
                             base = 16;
                             if (i < MAX_SCAN_WIDTH) buf[i++] = '0';
                             if (i < MAX_SCAN_WIDTH) buf[i++] = (char)next;
                             if (width > 0) width -= 2;
                             c = _in_char(stream);
                             if(c!=EOF) chars_consumed++;
                         } else {
                             if (base == 0) base = 8;
                             _unget_char(stream, next);
                             if(next!=EOF) chars_consumed--;
                         }
                    } else if (base == 0) {
                        base = 8;
                    }
                } else if (base == 0) {
                    base = 10;
                }

                while (c != EOF) {
                    if (width == 0) { _unget_char(stream, c); if(c!=EOF) chars_consumed--; break; }
                    int val = -1;
                    if (isdigit(c)) val = c - '0';
                    else if (isalpha(c)) val = tolower(c) - 'a' + 10;

                    if (val >= 0 && val < base) {
                        has_digit = 1;
                        if (i < MAX_SCAN_WIDTH) buf[i++] = (char)c;
                        if (width > 0) width--;
                        c = _in_char(stream);
                        if(c!=EOF) chars_consumed++;
                    } else {
                        _unget_char(stream, c);
                        if(c!=EOF) chars_consumed--;
                        break;
                    }
                }
                buf[i] = '\0';

                if (!has_digit) goto match_failure;

                if (!(flags & FL_SPLAT)) {
                    unsigned long long res = strtoull(buf, NULL, base);
                    if (conversion_char == 'd' || conversion_char == 'i') {
                        long long sres = (long long)res;
                        if (flags & FL_LONGLONG) *va_arg(arg, long long*) = sres;
                        else if (flags & FL_LONG) *va_arg(arg, long*) = (long)sres;
                        else if (flags & FL_SHORT) *va_arg(arg, short*) = (short)sres;
                        else if (flags & FL_SHORTSHORT) *va_arg(arg, char*) = (char)sres;
                        else *va_arg(arg, int*) = (int)sres;
                    } else {
                        if (flags & FL_LONGLONG) *va_arg(arg, unsigned long long*) = res;
                        else if (flags & FL_LONG) *va_arg(arg, unsigned long*) = (unsigned long)res;
                        else if (flags & FL_SHORT) *va_arg(arg, unsigned short*) = (unsigned short)res;
                        else if (flags & FL_SHORTSHORT) *va_arg(arg, unsigned char*) = (unsigned char)res;
                        else *va_arg(arg, unsigned int*) = (unsigned int)res;
                    }
                    nmatch++;
                }
                break;
            }

            case 'f': case 'e': case 'E': case 'g': case 'G': {
                i = 0;
                c = _in_char(stream);
                if (c != EOF) chars_consumed++;
                if (c == EOF) goto input_failure;

                if (c == '-' || c == '+') {
                    buf[i++] = c;
                    c = _in_char(stream);
                    if (c != EOF) chars_consumed++;
                    if(width>0) width--;
                }

                int digits = 0;
                int has_dot = 0, has_exp = 0;
                while (c != EOF) {
                    if (width == 0) { _unget_char(stream, c); if(c!=EOF) chars_consumed--; break; }
                    if (isdigit(c)) {
                        if (i < MAX_SCAN_WIDTH) buf[i++] = c;
                        digits++;
                    } else if (c == '.') {
                        if (has_dot || has_exp) { _unget_char(stream, c); if(c!=EOF) chars_consumed--; break; }
                        has_dot = 1;
                        if (i < MAX_SCAN_WIDTH) buf[i++] = c;
                    } else if (c == 'e' || c == 'E') {
                        if (has_exp || digits == 0) { _unget_char(stream, c); if(c!=EOF) chars_consumed--; break; }
                        has_exp = 1;
                        if (i < MAX_SCAN_WIDTH) buf[i++] = c;
                        c = _in_char(stream);
                        if (c != EOF) chars_consumed++;
                        if (c == '+' || c == '-') {
                             if (i < MAX_SCAN_WIDTH) buf[i++] = c;
                             if (width > 0) width--;
                        } else {
                            _unget_char(stream, c);
                            if(c!=EOF) chars_consumed--;
                        }
                    } else {
                        _unget_char(stream, c);
                        if(c!=EOF) chars_consumed--;
                        break;
                    }
                    if (width > 0) width--;
                    c = _in_char(stream);
                    if (c != EOF) chars_consumed++;
                }
                buf[i] = '\0';
                
                if (digits == 0) goto match_failure;

                if (!(flags & FL_SPLAT)) {
                    double val = strtod(buf, NULL);
                    if (flags & FL_LONG) *va_arg(arg, double*) = val;
                    else if (flags & FL_DOUBLE) *va_arg(arg, double*) = val;
                    else *va_arg(arg, float*) = (float)val;
                    nmatch++;
                }
                break;
            }

            case 's': {
                char *str = (flags & FL_SPLAT) ? NULL : va_arg(arg, char*);
                // Note: %s skips leading whitespace, which is handled by the block before switch
                c = _in_char(stream);
                if (c != EOF) chars_consumed++;
                if (c == EOF) goto input_failure;

                while (c != EOF && !isspace(c)) {
                    if (width == 0) { _unget_char(stream, c); if(c!=EOF) chars_consumed--; break; }
                    if (str) *str++ = (char)c;
                    if (width > 0) width--;
                    c = _in_char(stream);
                    if (c != EOF) chars_consumed++;
                }
                if (isspace(c)) { _unget_char(stream, c); if(c!=EOF) chars_consumed--; }
                if (str) { *str = '\0'; nmatch++; }
                break;
            }

            case 'c': {
                char *str = (flags & FL_SPLAT) ? NULL : va_arg(arg, char*);
                if (width == -1) width = 1;
                while (width-- > 0) {
                    c = _in_char(stream);
                    if (c != EOF) chars_consumed++;
                    if (c == EOF) { if (width == -1 && nmatch==0) goto input_failure; break; }
                    if (str) *str++ = (char)c;
                }
                if (!(flags & FL_SPLAT)) nmatch++;
                break;
            }

            case '[': {
                char *str = (flags & FL_SPLAT) ? NULL : va_arg(arg, char*);
                int invert = 0;
                char scanset[256];
                memset(scanset, 0, 256);

                if (*p == '^') { invert = 1; p++; }
                if (*p == ']') { scanset[']'] = 1; p++; }
                while (*p && *p != ']') {
                    if (*p == '-' && p[1] != ']' && p[-1] != (invert ? '^' : '[')) {
                        unsigned char start = (unsigned char)p[-1];
                        unsigned char end = (unsigned char)p[1];
                        int k;
                        for (k = start; k <= end; k++) scanset[k] = 1;
                        p += 2;
                    } else {
                        scanset[(unsigned char)*p] = 1;
                        p++;
                    }
                }
                if (*p == ']') p++;

                c = _in_char(stream);
                if (c != EOF) chars_consumed++;
                if (c == EOF) goto input_failure;
                
                int matched = 0;
                while (c != EOF) {
                    if (width == 0) { _unget_char(stream, c); if(c!=EOF) chars_consumed--; break; }
                    int m = scanset[(unsigned char)c];
                    if (invert) m = !m;
                    if (!m) { _unget_char(stream, c); if(c!=EOF) chars_consumed--; break; }
                    if (str) *str++ = (char)c;
                    matched++;
                    if (width > 0) width--;
                    c = _in_char(stream);
                    if (c != EOF) chars_consumed++;
                }
                
                if (matched == 0) goto match_failure;
                if (str) { *str = '\0'; nmatch++; }
                break;
            }
        }
    }

    if (!(stream->_flags & __S_STR)) _spin_unlock(&stream->_lock);
    return nmatch;

match_failure:
    if (!(stream->_flags & __S_STR)) _spin_unlock(&stream->_lock);
    return nmatch;

input_failure:
    if (!(stream->_flags & __S_STR)) _spin_unlock(&stream->_lock);
    return (nmatch == 0) ? EOF : nmatch;
}
