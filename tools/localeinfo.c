#include <arpa/inet.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    char magic[8];
    char encoding[32];
    uint32_t sgetrune_ptr;
    uint32_t sputrune_ptr;
    uint32_t invalid_rune;
    uint32_t runetype[256];
    uint32_t maplower[256];
    uint32_t mapupper[256];

    int32_t runetype_ext_nranges;
    uint32_t runetype_ext_ranges_ptr;
    int32_t maplower_ext_nranges;
    uint32_t maplower_ext_ranges_ptr;
    int32_t mapupper_ext_nranges;
    uint32_t mapupper_ext_ranges_ptr;

    uint32_t variable_ptr;
    int32_t variable_len;

    int32_t ncharclasses;
    uint32_t charclasses_ptr;
} FileRuneLocale;

typedef struct {
    uint32_t min;
    uint32_t max;
    int32_t map;
    uint32_t types_ptr;
} FileRuneEntry;
#pragma pack(pop)

static void print_help(const char *prog_name)
{
    printf("Usage: %s [OPTIONS] <LC_CTYPE_FILE>\n\n", prog_name);
    printf("Inspect and dump Apple-style LC_CTYPE locale files.\n\n");
    printf("Options:\n");
    printf("  --header    Show only header info (default if no other flags provided)\n");
    printf("  --base      Dump base range (0x00 - 0xFF)\n");
    printf("  --ext-rt    Dump extended Runetype ranges\n");
    printf("  --ext-ml    Dump extended Maplower ranges\n");
    printf("  --ext-mu    Dump extended Mapupper ranges\n");
    printf("  --all       Dump everything\n");
    printf("  -h, --help  Show this help\n");
}

static void format_mask(uint32_t mask, char *buf)
{
    buf[0] = '\0';
    if (mask == 0) {
        strcpy(buf, "NONE");
        return;
    }

    if (mask & RL_CTYPE_A)
        strcat(buf, "A|");
    if (mask & RL_CTYPE_C)
        strcat(buf, "C|");
    if (mask & RL_CTYPE_D)
        strcat(buf, "D|");
    if (mask & RL_CTYPE_G)
        strcat(buf, "G|");
    if (mask & RL_CTYPE_L)
        strcat(buf, "L|");
    if (mask & RL_CTYPE_P)
        strcat(buf, "P|");
    if (mask & RL_CTYPE_S)
        strcat(buf, "S|");
    if (mask & RL_CTYPE_U)
        strcat(buf, "U|");
    if (mask & RL_CTYPE_X)
        strcat(buf, "X|");
    if (mask & RL_CTYPE_B)
        strcat(buf, "B|");
    if (mask & RL_CTYPE_R)
        strcat(buf, "R|");

    size_t len = strlen(buf);
    if (len > 0 && buf[len - 1] == '|') {
        buf[len - 1] = '\0';
    }
}

int main(int argc, char *argv[])
{
    const char *filename = NULL;
    int show_header = 0, show_base = 0, show_ext_rt = 0, show_ext_ml = 0, show_ext_mu = 0;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            print_help(argv[0]);
            return EXIT_SUCCESS;
        } else if (strcmp(argv[i], "--header") == 0) {
            show_header = 1;
        } else if (strcmp(argv[i], "--base") == 0) {
            show_base = 1;
        } else if (strcmp(argv[i], "--ext-rt") == 0) {
            show_ext_rt = 1;
        } else if (strcmp(argv[i], "--ext-ml") == 0) {
            show_ext_ml = 1;
        } else if (strcmp(argv[i], "--ext-mu") == 0) {
            show_ext_mu = 1;
        } else if (strcmp(argv[i], "--all") == 0) {
            show_header = show_base = show_ext_rt = show_ext_ml = show_ext_mu = 1;
        } else if (argv[i][0] == '-') {
            fprintf(stderr, "Error: Unknown option '%s'.\n", argv[i]);
            return EXIT_FAILURE;
        } else {
            filename = argv[i];
        }
    }

    if (!filename) {
        fprintf(stderr, "Error: Missing input file.\n");
        return EXIT_FAILURE;
    }

    if (!show_header && !show_base && !show_ext_rt && !show_ext_ml && !show_ext_mu) {
        show_header = 1;
    }

    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        fprintf(stderr, "Error: Cannot open file '%s': %s\n", filename, strerror(errno));
        return EXIT_FAILURE;
    }

    FileRuneLocale frl;
    if (fread(&frl, sizeof(frl), 1, fp) != 1) {
        fprintf(stderr, "Error: Failed to read header from '%s'\n", filename);
        fclose(fp);
        return EXIT_FAILURE;
    }

    if (strncmp(frl.magic, "RuneMagA", 8) != 0) {
        fprintf(stderr, "Error: Invalid magic number. Not a valid LC_CTYPE file.\n");
        fclose(fp);
        return EXIT_FAILURE;
    }

    int32_t rt_nranges = ntohl(frl.runetype_ext_nranges);
    int32_t ml_nranges = ntohl(frl.maplower_ext_nranges);
    int32_t mu_nranges = ntohl(frl.mapupper_ext_nranges);

    FileRuneEntry *rt_ranges = NULL;
    FileRuneEntry *ml_ranges = NULL;
    FileRuneEntry *mu_ranges = NULL;

    if (rt_nranges > 0) {
        rt_ranges = malloc(rt_nranges * sizeof(FileRuneEntry));
        if (fread(rt_ranges, sizeof(FileRuneEntry), rt_nranges, fp) != (size_t)rt_nranges)
            goto read_err;
    }
    if (ml_nranges > 0) {
        ml_ranges = malloc(ml_nranges * sizeof(FileRuneEntry));
        if (fread(ml_ranges, sizeof(FileRuneEntry), ml_nranges, fp) != (size_t)ml_nranges)
            goto read_err;
    }
    if (mu_nranges > 0) {
        mu_ranges = malloc(mu_nranges * sizeof(FileRuneEntry));
        if (fread(mu_ranges, sizeof(FileRuneEntry), mu_nranges, fp) != (size_t)mu_nranges)
            goto read_err;
    }

    fclose(fp);

    if (show_header) {
        printf("--- Header ---\n");
        printf("Magic:         %.8s\n", frl.magic);
        printf("Encoding:      %s\n", frl.encoding);
        printf("Invalid Rune:  0x%04X\n", ntohl(frl.invalid_rune));
        printf("Ext Ranges:    RT: %d | MapL: %d | MapU: %d\n", rt_nranges, ml_nranges, mu_nranges);
        printf("\n");
    }

    if (show_base) {
        printf("--- Base Table (0x00 - 0xFF) ---\n");
        printf(" CODE | TYPE (MASK)         | LOWER  | UPPER  \n");
        printf("----------------------------------------------\n");
        char mask_buf[64];
        for (int i = 0; i < 256; i++) {
            uint32_t mask = ntohl(frl.runetype[i]);
            uint32_t lower = ntohl(frl.maplower[i]);
            uint32_t upper = ntohl(frl.mapupper[i]);

            if (mask == 0 && lower == (uint32_t)i && upper == (uint32_t)i)
                continue;

            format_mask(mask, mask_buf);
            printf(" %04X | %-19s | 0x%04X | 0x%04X \n", i, mask_buf, lower, upper);
        }
        printf("\n");
    }

    if (show_ext_rt && rt_nranges > 0) {
        printf("--- Extended Runetype Ranges ---\n");
        printf(" MIN    - MAX    | TYPE (MASK)\n");
        printf("----------------------------------\n");
        char mask_buf[64];
        for (int i = 0; i < rt_nranges; i++) {
            uint32_t mask = ntohl(rt_ranges[i].map);
            format_mask(mask, mask_buf);
            printf(
                " %06X - %06X | %s\n", ntohl(rt_ranges[i].min), ntohl(rt_ranges[i].max), mask_buf);
        }
        printf("\n");
    }

    if (show_ext_ml && ml_nranges > 0) {
        printf("--- Extended Maplower Ranges ---\n");
        printf(" MIN    - MAX    | MAP BASE\n");
        printf("----------------------------------\n");
        for (int i = 0; i < ml_nranges; i++) {
            printf(" %06X - %06X | 0x%06X\n",
                   ntohl(ml_ranges[i].min),
                   ntohl(ml_ranges[i].max),
                   ntohl(ml_ranges[i].map));
        }
        printf("\n");
    }

    if (show_ext_mu && mu_nranges > 0) {
        printf("--- Extended Mapupper Ranges ---\n");
        printf(" MIN    - MAX    | MAP BASE\n");
        printf("----------------------------------\n");
        for (int i = 0; i < mu_nranges; i++) {
            printf(" %06X - %06X | 0x%06X\n",
                   ntohl(mu_ranges[i].min),
                   ntohl(mu_ranges[i].max),
                   ntohl(mu_ranges[i].map));
        }
        printf("\n");
    }

    free(rt_ranges);
    free(ml_ranges);
    free(mu_ranges);
    return EXIT_SUCCESS;

read_err:
    fprintf(stderr, "Error: Unexpected end of file while reading extended ranges.\n");
    fclose(fp);
    free(rt_ranges);
    free(ml_ranges);
    free(mu_ranges);
    return EXIT_FAILURE;
}
