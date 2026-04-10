#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <errno.h>

#define RL_CTYPE_A 0x00000100L
#define RL_CTYPE_C 0x00000200L
#define RL_CTYPE_D 0x00000400L
#define RL_CTYPE_G 0x00000800L
#define RL_CTYPE_L 0x00001000L
#define RL_CTYPE_P 0x00002000L
#define RL_CTYPE_S 0x00004000L
#define RL_CTYPE_U 0x00008000L
#define RL_CTYPE_X 0x00010000L
#define RL_CTYPE_B 0x00020000L
#define RL_CTYPE_R 0x00040000L

#pragma pack(push, 4)
typedef struct {
    char        magic[8];
    char        encoding[32];
    uint32_t    sgetrune_ptr;
    uint32_t    sputrune_ptr;
    uint32_t    invalid_rune;
    uint32_t    runetype[256];
    uint32_t    maplower[256];
    uint32_t    mapupper[256];
    
    int32_t     runetype_ext_nranges;
    uint32_t    runetype_ext_ranges_ptr;
    int32_t     maplower_ext_nranges;
    uint32_t    maplower_ext_ranges_ptr;
    int32_t     mapupper_ext_nranges;
    uint32_t    mapupper_ext_ranges_ptr;

    uint32_t    variable_ptr;
    int32_t     variable_len;
    
    int32_t     ncharclasses;
    uint32_t    charclasses_ptr;
} FileRuneLocale;

typedef struct {
    uint32_t min;
    uint32_t max;
    int32_t  map;
    uint32_t types_ptr; 
} FileRuneEntry;
#pragma pack(pop)

#define MAX_UNICODE 0x110000

uint32_t type_map[MAX_UNICODE];
uint32_t lower_map[MAX_UNICODE];
uint32_t upper_map[MAX_UNICODE];

FileRuneEntry *rt_ranges = NULL; int rt_count = 0, rt_cap = 0;
FileRuneEntry *ml_ranges = NULL; int ml_count = 0, ml_cap = 0;
FileRuneEntry *mu_ranges = NULL; int mu_count = 0, mu_cap = 0;

static void print_help(const char *prog_name) {
    printf("Usage: %s [OPTIONS] <unicodedata.txt>\n\n", prog_name);
    printf("Generate Apple LC_CTYPE locale files from UnicodeData.txt.\n\n");
    printf("Options:\n");
    printf("  --size=<char|wchar>    Set encoding size (default: char).\n");
    printf("                           char  - Single-byte encoding (NONE)\n");
    printf("                           wchar - Multi-byte encoding (UTF-8)\n");
    printf("  --auto-ascii=<yes|no>  Hardcode POSIX ASCII fallback (0x00-0x7F) (default: yes).\n");
    printf("  -o, --output <file>    Specify output file name (default: LC_CTYPE).\n");
    printf("  -h, --help             Display this help message and exit.\n");
}

static void add_range(FileRuneEntry **arr, int *count, int *cap, uint32_t min, uint32_t max, int32_t map) {
    if (*count >= *cap) {
        *cap = *cap == 0 ? 1024 : *cap * 2;
        FileRuneEntry *new_arr = realloc(*arr, *cap * sizeof(FileRuneEntry));
        if (!new_arr) {
            fprintf(stderr, "Error: Memory allocation failed.\n");
            exit(EXIT_FAILURE);
        }
        *arr = new_arr;
    }
    (*arr)[*count].min = htonl(min);
    (*arr)[*count].max = htonl(max);
    (*arr)[*count].map = htonl((uint32_t)map);
    (*arr)[*count].types_ptr = 0;
    (*count)++;
}

static uint32_t parse_category(const char *cat) {
    uint32_t m = 0;
    if (cat[0] == 'L') {
        m |= RL_CTYPE_A | RL_CTYPE_R | RL_CTYPE_G;
        if (cat[1] == 'u') m |= RL_CTYPE_U;
        if (cat[1] == 'l') m |= RL_CTYPE_L;
    } else if (cat[0] == 'N' && cat[1] == 'd') {
        m |= RL_CTYPE_D | RL_CTYPE_R | RL_CTYPE_G;
    } else if (cat[0] == 'N') {
        m |= RL_CTYPE_R | RL_CTYPE_G;
    } else if (cat[0] == 'P' || cat[0] == 'S') {
        m |= RL_CTYPE_P | RL_CTYPE_R | RL_CTYPE_G;
    } else if (cat[0] == 'Z') {
        m |= RL_CTYPE_S | RL_CTYPE_R;
        if (cat[1] == 's') m |= RL_CTYPE_B;
    } else if (cat[0] == 'C') {
        m |= RL_CTYPE_C;
    }
    return m;
}

static int parse_unicodedata(const char *filename, int auto_ascii) {
    FILE *f = fopen(filename, "r");
    if (!f) {
        fprintf(stderr, "Error: Cannot open input file '%s': %s\n", filename, strerror(errno));
        return -1;
    }

    for (int i = 0; i < MAX_UNICODE; i++) {
        type_map[i] = 0;
        lower_map[i] = i;
        upper_map[i] = i;
    }

    char line[1024];
    while (fgets(line, sizeof(line), f)) {
        char *fields[15];
        int count = 0;
        char *ptr = line;
        fields[count++] = ptr;
        
        while (*ptr) {
            if (*ptr == ';') {
                *ptr = '\0';
                fields[count++] = ptr + 1;
            }
            ptr++;
        }
        
        if (count < 14) continue;

        uint32_t cp = strtol(fields[0], NULL, 16);
        if (cp >= MAX_UNICODE) continue;

        type_map[cp] = parse_category(fields[2]);

        if (fields[13][0]) lower_map[cp] = strtol(fields[13], NULL, 16);
        if (fields[12][0]) upper_map[cp] = strtol(fields[12], NULL, 16);
    }
    fclose(f);

    if (auto_ascii) {
        for (int i = 0; i < 128; i++) {
            uint32_t m = 0;
            if ((i >= 0x00 && i <= 0x1F) || i == 0x7F) m |= RL_CTYPE_C;
            if (i >= 0x09 && i <= 0x0D) m |= RL_CTYPE_S;
            if (i == ' ') m |= RL_CTYPE_S | RL_CTYPE_B | RL_CTYPE_R;
            if (i == '\t') m |= RL_CTYPE_B;
            if (i >= 0x21 && i <= 0x7E) m |= RL_CTYPE_R | RL_CTYPE_G;
            
            if (i >= 'a' && i <= 'z') {
                m |= RL_CTYPE_L | RL_CTYPE_A;
                if (i >= 'a' && i <= 'f') m |= RL_CTYPE_X;
                upper_map[i] = i - 32;
            } else if (i >= 'A' && i <= 'Z') {
                m |= RL_CTYPE_U | RL_CTYPE_A;
                if (i >= 'A' && i <= 'F') m |= RL_CTYPE_X;
                lower_map[i] = i + 32;
            } else if (i >= '0' && i <= '9') {
                m |= RL_CTYPE_D | RL_CTYPE_X;
            }
            
            if ((m & RL_CTYPE_G) && !(m & (RL_CTYPE_A | RL_CTYPE_D))) m |= RL_CTYPE_P;
            
            type_map[i] = m;
        }
    }
    
    return 0;
}

int main(int argc, char *argv[]) {
    int is_wchar = 0;
    int auto_ascii = 1;
    const char *input_file = NULL;
    const char *output_file = "LC_CTYPE";

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            print_help(argv[0]);
            return EXIT_SUCCESS;
        } else if (strncmp(argv[i], "--size=", 7) == 0) {
            const char *val = argv[i] + 7;
            if (strcmp(val, "wchar") == 0) {
                is_wchar = 1;
            } else if (strcmp(val, "char") == 0) {
                is_wchar = 0;
            } else {
                fprintf(stderr, "Error: Invalid value for --size. Expected 'char' or 'wchar', got '%s'.\n", val);
                return EXIT_FAILURE;
            }
        } else if (strncmp(argv[i], "--auto-ascii=", 13) == 0) {
            const char *val = argv[i] + 13;
            if (strcmp(val, "yes") == 0) {
                auto_ascii = 1;
            } else if (strcmp(val, "no") == 0) {
                auto_ascii = 0;
            } else {
                fprintf(stderr, "Error: Invalid value for --auto-ascii. Expected 'yes' or 'no', got '%s'.\n", val);
                return EXIT_FAILURE;
            }
        } else if (strcmp(argv[i], "-o") == 0 || strcmp(argv[i], "--output") == 0) {
            if (i + 1 < argc) {
                output_file = argv[++i];
            } else {
                fprintf(stderr, "Error: Option '%s' requires an argument.\n", argv[i]);
                return EXIT_FAILURE;
            }
        } else if (argv[i][0] == '-') {
            fprintf(stderr, "Error: Unknown option '%s'.\n", argv[i]);
            fprintf(stderr, "Try '%s --help' for more information.\n", argv[0]);
            return EXIT_FAILURE;
        } else {
            if (input_file != NULL) {
                fprintf(stderr, "Error: Multiple input files are not supported.\n");
                return EXIT_FAILURE;
            }
            input_file = argv[i];
        }
    }

    if (!input_file) {
        fprintf(stderr, "Error: Missing input file.\n");
        fprintf(stderr, "Try '%s --help' for more information.\n", argv[0]);
        return EXIT_FAILURE;
    }

    if (parse_unicodedata(input_file, auto_ascii) != 0) {
        return EXIT_FAILURE;
    }

    FileRuneLocale frl;
    memset(&frl, 0, sizeof(frl));
    memcpy(frl.magic, "RuneMagA", 8);
    frl.invalid_rune = htonl(0xFFFD);

    for (int i = 0; i < 256; i++) {
        frl.runetype[i] = htonl(type_map[i]);
        frl.maplower[i] = htonl(lower_map[i]);
        frl.mapupper[i] = htonl(upper_map[i]);
    }

    if (!is_wchar) {
        strncpy(frl.encoding, "NONE", sizeof(frl.encoding) - 1);
    } else {
        strncpy(frl.encoding, "UTF-8", sizeof(frl.encoding) - 1);
        
        uint32_t start = 256;
        while (start < MAX_UNICODE) {
            if (type_map[start] == 0) { start++; continue; }
            uint32_t mask = type_map[start];
            uint32_t end = start;
            while (end + 1 < MAX_UNICODE && type_map[end + 1] == mask) end++;
            add_range(&rt_ranges, &rt_count, &rt_cap, start, end, mask);
            start = end + 1;
        }

        start = 256;
        while (start < MAX_UNICODE) {
            if (lower_map[start] == start) { start++; continue; }
            int32_t delta = (int32_t)lower_map[start] - (int32_t)start;
            uint32_t end = start;
            while (end + 1 < MAX_UNICODE && 
                   lower_map[end + 1] != (end + 1) &&
                   ((int32_t)lower_map[end + 1] - (int32_t)(end + 1)) == delta) {
                end++;
            }
            add_range(&ml_ranges, &ml_count, &ml_cap, start, end, lower_map[start]);
            start = end + 1;
        }

        start = 256;
        while (start < MAX_UNICODE) {
            if (upper_map[start] == start) { start++; continue; }
            int32_t delta = (int32_t)upper_map[start] - (int32_t)start;
            uint32_t end = start;
            while (end + 1 < MAX_UNICODE && 
                   upper_map[end + 1] != (end + 1) &&
                   ((int32_t)upper_map[end + 1] - (int32_t)(end + 1)) == delta) {
                end++;
            }
            add_range(&mu_ranges, &mu_count, &mu_cap, start, end, upper_map[start]);
            start = end + 1;
        }

        frl.runetype_ext_nranges = htonl(rt_count);
        frl.maplower_ext_nranges = htonl(ml_count);
        frl.mapupper_ext_nranges = htonl(mu_count);
    }

    FILE *fp = fopen(output_file, "wb");
    if (!fp) {
        fprintf(stderr, "Error: Cannot create output file '%s': %s\n", output_file, strerror(errno));
        goto cleanup;
    }

    if (fwrite(&frl, sizeof(frl), 1, fp) != 1) goto write_error;
    if (is_wchar) {
        if (rt_count > 0 && fwrite(rt_ranges, sizeof(FileRuneEntry), rt_count, fp) != (size_t)rt_count) goto write_error;
        if (ml_count > 0 && fwrite(ml_ranges, sizeof(FileRuneEntry), ml_count, fp) != (size_t)ml_count) goto write_error;
        if (mu_count > 0 && fwrite(mu_ranges, sizeof(FileRuneEntry), mu_count, fp) != (size_t)mu_count) goto write_error;
    }
    
    fclose(fp);

cleanup:
    free(rt_ranges);
    free(ml_ranges);
    free(mu_ranges);
    return EXIT_SUCCESS;

write_error:
    fprintf(stderr, "Error: Failed to write to file '%s': %s\n", output_file, strerror(errno));
    fclose(fp);
    remove(output_file);
    free(rt_ranges);
    free(ml_ranges);
    free(mu_ranges);
    return EXIT_FAILURE;
}
