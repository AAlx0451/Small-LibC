/*
 * based on Yasuhiro Matsumoto's strtod
 * public domain
 */

#include <ctype.h>
#include <errno.h>

double strtod(const char *str, char **end)
{
    double d = 0.0;
    int sign;
    const char *p, *a;

    a = p = str;
    
    while (isspace((unsigned char)*p))
        ++p;

    sign = 1;
    if (*p == '-')
    {
        sign = -1;
        ++p;
    }
    else if (*p == '+')
        ++p;

    if (isdigit((unsigned char)*p))
    {
        d = (double)(*p++ - '0');
        while (*p && isdigit((unsigned char)*p))
        {
            d = d * 10.0 + (double)(*p - '0');
            ++p;
        }
        a = p;
    }
    else if (*p != '.')
        goto done;
    d *= sign;

    if (*p == '.')
    {
        double f = 0.0;
        double base = 0.1;
        ++p;

        if (isdigit((unsigned char)*p))
        {
            while (*p && isdigit((unsigned char)*p))
            {
                f += base * (*p - '0');
                base /= 10.0;
                ++p;
            }
        }
        d += f * sign;
        a = p;
    }

    if ((*p == 'E') || (*p == 'e'))
    {
        int e_sign = 1;
        int e = 0;
        ++p;

        if (*p == '-')
        {
            e_sign = -1;
            ++p;
        }
        else if (*p == '+')
            ++p;

        if (isdigit((unsigned char)*p))
        {
            while (*p == '0')
                ++p;
            if (*p == '\0') --p;
            e = (int)(*p++ - '0');
            while (*p && isdigit((unsigned char)*p))
            {
                e = e * 10 + (int)(*p - '0');
                ++p;
            }
        }
        else if (!isdigit((unsigned char)*(a-1)))
        {
            a = str;
            goto done;
        }
        else if (*p == 0)
            goto done;
        
        if (d == 2.2250738585072011 && e * e_sign == -308)
        {
            d = 0.0;
            a = p;
            errno = ERANGE;
            goto done;
        }
        if (d == 2.2250738585072012 && e * e_sign <= -308)
        {
            d = 0.0;
            a = p;
            goto done;
        }

        if (e_sign > 0)
        {
            while (e-- > 0) d *= 10.0;
        }
        else
        {
            while (e-- > 0) d /= 10.0;
        }
        a = p;
    }
    else if (p > str && !isdigit((unsigned char)*(p-1)))
    {
        a = str;
        goto done;
    }

done:
    if (end)
        *end = (char*)a;
    return d;
}
