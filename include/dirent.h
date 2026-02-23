#ifndef DIRENT_H
#define DIRENT_H

#include <features.h>

#if !defined(_ANSI) && (defined(_POSIX_C_SOURCE) || defined(_XOPEN_SOURCE))

#include <stdint.h>

#define DT_UNKNOWN 0
#define DT_FIFO    1
#define DT_CHR     2
#define DT_DIR     4
#define DT_BLK     6
#define DT_REG     8
#define DT_LNK     10
#define DT_SOCK    12
#define DT_WHT     14

#pragma pack(push, 4)

struct dirent {
    uint64_t d_ino;
    uint64_t d_seekoff;
    uint16_t d_reclen;
    uint16_t d_namlen;
    uint8_t  d_type;
    char     d_name[256];
};

#pragma pack(pop)

typedef struct DIR DIR;

DIR *opendir(const char *);
struct dirent *readdir(DIR *);
int closedir(DIR *);
void rewinddir(DIR *);

#endif /* !ANSI && (_POSIX_C_SOURCE || _XOPEN_SOURCE) */

#endif /* !DIRENT_H */
