#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <string.h>

#define PAD_RIGHT 1
#define PAD_ZERO  2
#define PRINT_SGN 4
#define PRINT_SPC 8
#define PRINT_HEX_LO 16
#define PRINT_HEX_UP 32
#define PRINT_OCT 64

extern void _spin_lock(volatile int *lock);
extern void _spin_unlock(volatile int *lock);

static int _out_char(FILE *f, int c) {
    if (f->_flags & __S_STR) {
        if (f->_cnt > 0) {
            *f->_ptr++ = (unsigned char)c;
            f->_cnt--;
        }
        return 1;
    } else {
        if (f->_cnt > 0) {
            *f->_ptr++ = (unsigned char)c;
            f->_cnt--;
            f->_flags |= __S_DIRTY; /* <--- ВАЖНОЕ ИСПРАВЛЕНИЕ */
            return 1;
        }
        /* Если буфер полон, flush сам сбросит флаг dirty, запишет данные, 
           и мы начнем заполнять его заново */
        if (__stdio_flush_impl(f) == 0) {
            *f->_ptr++ = (unsigned char)c;
            f->_cnt--;
            f->_flags |= __S_DIRTY; /* <--- И ЗДЕСЬ */
            return 1;
        }
    }
    return 0;
}

static int _itoa_base(uintmax_t val, int base, int flags, char *buf) {
    char *orig = buf;
    const char *digits = (flags & PRINT_HEX_UP) ? "0123456789ABCDEF" : "0123456789abcdef";
    
    if (val == 0) {
        *buf++ = '0';
        return 1;
    }

    while (val) {
        *buf++ = digits[val % base];
        val /= base;
    }
    return buf - orig;
}

static int _ftoa(double val, int prec, int flags, char *buf) {
    char *orig = buf;
    int64_t ipart;
    double fpart;
    int i;
    
    if (val < 0) {
        val = -val;
    }

    ipart = (int64_t)val;
    fpart = val - (double)ipart;

    buf += _itoa_base(ipart, 10, 0, buf);
    
    if (prec > 0) {
        *buf++ = '.';
        for (i = 0; i < prec; i++) {
            fpart *= 10.0;
        }
        fpart += 0.5;
        ipart = (int64_t)fpart;
        
        char *f_start = buf;
        int len = _itoa_base(ipart, 10, 0, buf);
        buf += len;
        
        while (len < prec) {
            *buf++ = '0'; 
            len++;
        }
        
        if (len > prec) {
             
        } else {
             int j;
             for (j = 0; j < len / 2; j++) {
                 char tmp = f_start[j];
                 f_start[j] = f_start[len - 1 - j];
                 f_start[len - 1 - j] = tmp;
             }
        }
    }
    
    return buf - orig;
}

static void _reverse(char *s, int len) {
    int i, j;
    char c;
    for (i = 0, j = len - 1; i < j; i++, j--) {
        c = s[i]; s[i] = s[j]; s[j] = c;
    }
}

int vfprintf(FILE *stream, const char *format, va_list arg) {
    const char *p = format;
    int total_written = 0;
    char temp_buf[128];
    
    if (!stream) return -1;
    
    if (!(stream->_flags & __S_STR)) {
        _spin_lock(&stream->_lock);
    }

    while (*p) {
        if (*p != '%') {
            total_written += _out_char(stream, *p++);
            continue;
        }

        p++; 
        int flags = 0;
        int width = 0;
        int prec = -1;
        int len_mod = 0; 
        
        while (1) {
            if (*p == '-') flags |= PAD_RIGHT;
            else if (*p == '+') flags |= PRINT_SGN;
            else if (*p == ' ') flags |= PRINT_SPC;
            else if (*p == '#') ; 
            else if (*p == '0') flags |= PAD_ZERO;
            else break;
            p++;
        }

        if (*p == '*') {
            width = va_arg(arg, int);
            if (width < 0) { flags |= PAD_RIGHT; width = -width; }
            p++;
        } else {
            while (*p >= '0' && *p <= '9') {
                width = width * 10 + (*p - '0');
                p++;
            }
        }

        if (*p == '.') {
            p++;
            if (*p == '*') {
                prec = va_arg(arg, int);
                p++;
            } else {
                prec = 0;
                while (*p >= '0' && *p <= '9') {
                    prec = prec * 10 + (*p - '0');
                    p++;
                }
            }
        }

        if (*p == 'h') { len_mod = 1; p++; if (*p == 'h') { len_mod = 2; p++; } }
        else if (*p == 'l') { len_mod = 3; p++; if (*p == 'l') { len_mod = 4; p++; } }
        else if (*p == 'L') { len_mod = 5; p++; }
        else if (*p == 'z') { len_mod = 6; p++; }
        else if (*p == 't') { len_mod = 7; p++; }

        char type = *p++;
        char *str_val = NULL;
        int slen = 0;
        uintmax_t uval = 0;
        intmax_t sval = 0;
        double fval = 0.0;
        int base = 10;
        int is_signed = 0;
        int is_float = 0;
        
        switch (type) {
            case 'd':
            case 'i':
                is_signed = 1;
                if (len_mod == 4) sval = va_arg(arg, long long);
                else if (len_mod == 3) sval = va_arg(arg, long);
                else if (len_mod == 6) sval = va_arg(arg, ssize_t);
                else sval = va_arg(arg, int);
                
                if (sval < 0) { uval = -sval; } else { uval = sval; }
                break;
            case 'u':
                if (len_mod == 4) uval = va_arg(arg, unsigned long long);
                else if (len_mod == 3) uval = va_arg(arg, unsigned long);
                else if (len_mod == 6) uval = va_arg(arg, size_t);
                else uval = va_arg(arg, unsigned int);
                break;
            case 'o':
                base = 8;
                flags |= PRINT_OCT;
                uval = va_arg(arg, unsigned int); 
                break;
            case 'x':
                base = 16;
                flags |= PRINT_HEX_LO;
                uval = va_arg(arg, unsigned int);
                break;
            case 'X':
                base = 16;
                flags |= PRINT_HEX_UP;
                uval = va_arg(arg, unsigned int);
                break;
            case 'c':
                temp_buf[0] = (char)va_arg(arg, int);
                slen = 1;
                str_val = temp_buf;
                break;
            case 's':
                str_val = va_arg(arg, char*);
                if (!str_val) str_val = "(null)";
                slen = strlen(str_val);
                if (prec >= 0 && slen > prec) slen = prec;
                break;
            case 'p':
                uval = (uintptr_t)va_arg(arg, void*);
                base = 16;
                flags |= PRINT_HEX_LO | PAD_ZERO;
                if (width == 0) width = sizeof(void*) * 2 + 2; 
                str_val = temp_buf;
                temp_buf[0] = '0'; temp_buf[1] = 'x';
                slen = 2 + _itoa_base(uval, 16, flags, temp_buf + 2);
                _reverse(temp_buf + 2, slen - 2);
                break;
            case 'f':
            case 'F':
            case 'g':
            case 'G':
                is_float = 1;
                fval = va_arg(arg, double);
                if (prec < 0) prec = 6;
                if (fval < 0) { flags |= PRINT_SGN; } 
                slen = _ftoa(fval, prec, flags, temp_buf);
                if (type == 'g' || type == 'G') {
                     
                    while (slen > 0 && temp_buf[0] == '0') {
                         
                        char *ptr = temp_buf;
                        int i;
                        for(i=0; i<slen-1; i++) ptr[i] = ptr[i+1];
                        slen--;
                    }
                } else {
                   _reverse(temp_buf, slen);
                }
                str_val = temp_buf;
                break;
            case '%':
                temp_buf[0] = '%';
                slen = 1;
                str_val = temp_buf;
                break;
            default:
                break;
        }

        if (base != 10 || type == 'u' || type == 'd' || type == 'i') {
             slen = _itoa_base(uval, base, flags, temp_buf);
             _reverse(temp_buf, slen);
             str_val = temp_buf;
        }

        int pad_len = width - slen;
        int sgn_len = 0;
        char sgn_char = 0;

        if (is_signed && (flags & PRINT_SGN)) sgn_char = '+';
        else if (is_signed && (flags & PRINT_SPC)) sgn_char = ' ';
        else if (is_signed && sval < 0) sgn_char = '-';
        else if (is_float && fval < 0) sgn_char = '-';
        
        if (sgn_char) {
            sgn_len = 1;
            pad_len--;
        }

        if (!(flags & PAD_RIGHT)) {
            if (flags & PAD_ZERO) {
                if (sgn_char) total_written += _out_char(stream, sgn_char);
                while (pad_len-- > 0) total_written += _out_char(stream, '0');
            } else {
                while (pad_len-- > 0) total_written += _out_char(stream, ' ');
                if (sgn_char) total_written += _out_char(stream, sgn_char);
            }
        } else {
            if (sgn_char) total_written += _out_char(stream, sgn_char);
        }

        int i;
        for (i = 0; i < slen; i++) total_written += _out_char(stream, str_val[i]);

        if (flags & PAD_RIGHT) {
            while (pad_len-- > 0) total_written += _out_char(stream, ' ');
        }
    }
    
    if (!(stream->_flags & __S_STR)) {
        _spin_unlock(&stream->_lock);
    }
    
    return total_written;
}
