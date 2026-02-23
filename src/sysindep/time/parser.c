#include <fcntl.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

/* --- INTERNAL HELPERS --- */

static inline uint32_t __bswap32(uint32_t x) {
    return ((x & 0xFF000000u) >> 24) |
           ((x & 0x00FF0000u) >> 8) |
           ((x & 0x0000FF00u) << 8) |
           ((x & 0x000000FFu) << 24);
}

static inline uint64_t __bswap64(uint64_t x) {
    return ((x & 0xFF00000000000000ull) >> 56) |
           ((x & 0x00FF000000000000ull) >> 40) |
           ((x & 0x0000FF0000000000ull) >> 24) |
           ((x & 0x000000FF00000000ull) >> 8) |
           ((x & 0x00000000FF000000ull) << 8) |
           ((x & 0x0000000000FF0000ull) << 24) |
           ((x & 0x000000000000FF00ull) << 40) |
           ((x & 0x00000000000000FFull) << 56);
}

static inline uint32_t __be32_to_cpu(uint32_t x) {
    const uint16_t endian_check = 0x0001;
    if(*((const uint8_t *)&endian_check)) {
        return __bswap32(x);
    }
    return x;
}

static inline uint64_t __be64_to_cpu(uint64_t x) {
    const uint16_t endian_check = 0x0001;
    if(*((const uint8_t *)&endian_check)) {
        return __bswap64(x);
    }
    return x;
}

/* --- DATA STRUCTURES --- */

#pragma pack(push, 1)

struct __tz_header {
    char magic[4];
    char version;
    char reserved[15];
    uint32_t tzh_ttisgmtcnt;
    uint32_t tzh_ttisstdcnt;
    uint32_t tzh_leapcnt;
    uint32_t tzh_timecnt;
    uint32_t tzh_typecnt;
    uint32_t tzh_charcnt;
};

struct __ttinfo_raw {
    int32_t tt_gmtoff;
    uint8_t tt_isdst;
    uint8_t tt_abbrind;
};

#pragma pack(pop)

typedef struct
{
    int32_t gmtoff;
    int is_dst;
    uint8_t abbr_idx;
} __ttinfo_t;

typedef struct
{
    struct __tz_header header;
    int64_t *transitions;
    uint8_t *type_idxs;
    __ttinfo_t *types;
    char *abbrevs;
} tz_db_t;

typedef struct
{
    int32_t gmtoff;
    int is_dst;
    const char *abbr;
} tz_result_t;

/* --- PRIVATE IMPLEMENTATION --- */

static void __tz_host_header(struct __tz_header *h) {
    h->tzh_ttisgmtcnt = __be32_to_cpu(h->tzh_ttisgmtcnt);
    h->tzh_ttisstdcnt = __be32_to_cpu(h->tzh_ttisstdcnt);
    h->tzh_leapcnt = __be32_to_cpu(h->tzh_leapcnt);
    h->tzh_timecnt = __be32_to_cpu(h->tzh_timecnt);
    h->tzh_typecnt = __be32_to_cpu(h->tzh_typecnt);
    h->tzh_charcnt = __be32_to_cpu(h->tzh_charcnt);
}

static int __tz_skip_data(int fd, struct __tz_header *h) {
    off_t skip = (off_t)h->tzh_timecnt * 5 +
                 (off_t)h->tzh_typecnt * 6 +
                 (off_t)h->tzh_charcnt +
                 (off_t)h->tzh_leapcnt * 8 +
                 (off_t)h->tzh_ttisstdcnt +
                 (off_t)h->tzh_ttisgmtcnt;

    /* lseek returns -1 on error, or the new offset on success */
    if(lseek(fd, skip, SEEK_CUR) == (off_t)-1) {
        return -1;
    }
    return 0;
}

static int __tz_parse_body(int fd, tz_db_t *db, int is_v2) {
    uint32_t i;

    db->transitions = malloc(db->header.tzh_timecnt * sizeof(int64_t));
    db->type_idxs = malloc(db->header.tzh_timecnt * sizeof(uint8_t));
    db->types = malloc(db->header.tzh_typecnt * sizeof(__ttinfo_t));
    db->abbrevs = malloc(db->header.tzh_charcnt * sizeof(char));

    if(!db->transitions || !db->type_idxs || !db->types || !db->abbrevs) {
        return -1;
    }

    if(is_v2) {
        for(i = 0; i < db->header.tzh_timecnt; i++) {
            /* read returns number of bytes read */
            if(read(fd, &db->transitions[i], 8) != 8)
                return -1;
            db->transitions[i] = (int64_t)__be64_to_cpu((uint64_t)db->transitions[i]);
        }
    } else {
        for(i = 0; i < db->header.tzh_timecnt; i++) {
            int32_t t32;
            if(read(fd, &t32, 4) != 4)
                return -1;
            db->transitions[i] = (int64_t)((int32_t)__be32_to_cpu(t32));
        }
    }

    if(read(fd, db->type_idxs, db->header.tzh_timecnt) != (ssize_t)db->header.tzh_timecnt) {
        return -1;
    }

    for(i = 0; i < db->header.tzh_typecnt; i++) {
        struct __ttinfo_raw raw;
        if(read(fd, &raw, sizeof(raw)) != sizeof(raw))
            return -1;

        db->types[i].gmtoff = (int32_t)__be32_to_cpu((uint32_t)raw.tt_gmtoff);
        db->types[i].is_dst = raw.tt_isdst;
        db->types[i].abbr_idx = raw.tt_abbrind;
    }

    if(read(fd, db->abbrevs, db->header.tzh_charcnt) != (ssize_t)db->header.tzh_charcnt) {
        return -1;
    }

    return 0;
}

/* --- PUBLIC API --- */

void tz_destroy(tz_db_t *db) {
    if(db) {
        free(db->transitions);
        free(db->type_idxs);
        free(db->types);
        free(db->abbrevs);
        free(db);
    }
}

tz_db_t *tz_load(const char *filepath) {
    int fd = open(filepath, O_RDONLY);
    if(fd < 0)
        return NULL;

    tz_db_t *db = calloc(1, sizeof(tz_db_t));
    if(!db) {
        close(fd);
        return NULL;
    }

    if(read(fd, &db->header, sizeof(struct __tz_header)) != sizeof(struct __tz_header)) {
        goto error;
    }

    if(memcmp(db->header.magic, "TZif", 4) != 0) {
        goto error;
    }

    int version = (db->header.version >= '2') ? 2 : 1;
    __tz_host_header(&db->header);

    if(version == 2) {
        if(__tz_skip_data(fd, &db->header) != 0)
            goto error;

        if(read(fd, &db->header, sizeof(struct __tz_header)) != sizeof(struct __tz_header))
            goto error;
        if(memcmp(db->header.magic, "TZif", 4) != 0)
            goto error;

        __tz_host_header(&db->header);
    }

    if(__tz_parse_body(fd, db, (version == 2)) != 0) {
        goto error;
    }

    close(fd);
    return db;

error:
    close(fd);
    tz_destroy(db);
    return NULL;
}

int tz_lookup(const tz_db_t *db, int64_t timestamp, tz_result_t *result) {
    if(!db || !result)
        return 0;

    int idx = -1;

    if(db->header.tzh_timecnt > 0) {
        if(timestamp < db->transitions[0]) {
            idx = -1;
        } else {
            uint32_t low = 0;
            uint32_t high = db->header.tzh_timecnt - 1;

            while(low <= high) {
                uint32_t mid = low + (high - low) / 2;
                if(db->transitions[mid] <= timestamp) {
                    idx = mid;
                    low = mid + 1;
                } else {
                    if(mid == 0)
                        break;
                    high = mid - 1;
                }
            }
        }
    }

    int type_idx;

    if(idx != -1) {
        type_idx = db->type_idxs[idx];
    } else {
        type_idx = 0;
        for(uint32_t i = 0; i < db->header.tzh_typecnt; i++) {
            if(!db->types[i].is_dst) {
                type_idx = i;
                break;
            }
        }
    }

    if((uint32_t)type_idx >= db->header.tzh_typecnt)
        return 0;

    const __ttinfo_t *info = &db->types[type_idx];
    result->gmtoff = info->gmtoff;
    result->is_dst = info->is_dst;
    result->abbr = &db->abbrevs[info->abbr_idx];

    return 1;
}
