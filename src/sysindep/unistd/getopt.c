#include <string.h>
#include <unistd.h>

#pragma clang diagnostic ignored "-Wunknown-warning-option"
#pragma clang diagnostic ignored "-Wreserved-identifier"

char *optarg = NULL;
int optind = 1;
int opterr = 1;
int optopt = 0;

static char *scan = NULL;

static void _err_write(const char *s) {
    if(s) {
        size_t len = 0;
        const char *p = s;
        while(*p++)
            len++;
        write(STDERR_FILENO, s, len);
    }
}

int getopt(int argc, char *const argv[], const char *optstring) {
    char c, ch[2];
    char *place;

    optarg = NULL;

    if(!scan || !*scan) {
        if(optind == 0)
            optind++;

        if(optind >= argc || !argv[optind])
            return -1;

        if(argv[optind][0] != '-' || argv[optind][1] == '\0')
            return -1;

        if(argv[optind][1] == '-' && argv[optind][2] == '\0') {
            optind++;
            return -1;
        }

        scan = argv[optind] + 1;
        optind++;
    }

    c = *scan++;
    optopt = c;
    place = strchr(optstring, c);

    if(!place || c == ':') {
        if(opterr && *optstring != ':') {
            _err_write(argv[0]);
            _err_write(": illegal option -- ");
            ch[0] = c;
            ch[1] = '\0';
            _err_write(ch);
            _err_write("\n");
        }
        return '?';
    }

    if(place[1] == ':') {
        if(*scan != '\0') {
            optarg = scan;
            scan = NULL;
        } else if(optind < argc) {
            optarg = argv[optind];
            optind++;
            scan = NULL;
        } else {
            if(opterr && *optstring != ':') {
                _err_write(argv[0]);
                _err_write(": option requires an argument -- ");
                ch[0] = c;
                ch[1] = '\0';
                _err_write(ch);
                _err_write("\n");
            }
            return (*optstring == ':') ? ':' : '?';
        }
    }

    return c;
}
