#ifndef TZ_CONTEXT_H
#define TZ_CONTEXT_H

#include <time.h>
#include <stdint.h>

#pragma pack(push, 1)
struct __tz_header_decl {
    char     magic[4];
    char     version;
    char     reserved[15];
    uint32_t tzh_ttisgmtcnt;
    uint32_t tzh_ttisstdcnt;
    uint32_t tzh_leapcnt;
    uint32_t tzh_timecnt;
    uint32_t tzh_typecnt;
    uint32_t tzh_charcnt;
};
#pragma pack(pop)

typedef struct {
    int32_t     gmtoff;
    int         is_dst;
    uint8_t     abbr_idx;
    uint8_t __padding[3];
} __ttinfo_t_decl;

typedef struct {
    struct __tz_header_decl header;
    int64_t*           transitions;
    uint8_t*           type_idxs;
    __ttinfo_t_decl*   types;
    char*              abbrevs;
} tz_db_t;

typedef struct {
    int32_t     gmtoff;
    int         is_dst;
    const char* abbr;
} tz_result_t;

tz_db_t* tz_load(const char* filepath);
void tz_destroy(tz_db_t* db);
int tz_lookup(const tz_db_t* db, int64_t timestamp, tz_result_t* result);

extern char *tzname[2];
extern long timezone;
extern int daylight;

extern tz_db_t *__libc_current_tz_db;

#endif
