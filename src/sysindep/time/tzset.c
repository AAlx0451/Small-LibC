#include "tz_context.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>

static char __tzname_std[16] = "GMT";
static char __tzname_dst[16] = "GMT";

char *tzname[2] = {__tzname_std, __tzname_dst};
long timezone = 0;
int daylight = 0;

tz_db_t *__libc_current_tz_db = NULL;

#define DEFAULT_TZ_PATH "/var/db/timezone/localtime"

void tzset(void) {
    const char *tz_env = getenv("TZ");
    const char *path = DEFAULT_TZ_PATH;

    if(__libc_current_tz_db) {
        tz_destroy(__libc_current_tz_db);
        __libc_current_tz_db = NULL;
    }

    if(tz_env && *tz_env) {
        path = tz_env;
    }

    __libc_current_tz_db = tz_load(path);

    if(!__libc_current_tz_db) {
        strcpy(__tzname_std, "GMT");
        strcpy(__tzname_dst, "GMT");
        timezone = 0;
        daylight = 0;
        return;
    }
    {
        uint32_t i;
        int found_std = 0;
        int found_dst = 0;

        daylight = 0;
        for(i = 0; i < __libc_current_tz_db->header.tzh_typecnt; i++) {
            const __ttinfo_t_decl *info = &__libc_current_tz_db->types[i];
            const char *abbr = &__libc_current_tz_db->abbrevs[info->abbr_idx];

            if(!info->is_dst) {
                if(!found_std) {
                    timezone = -info->gmtoff;
                    strncpy(__tzname_std, abbr, sizeof(__tzname_std) - 1);
                    found_std = 1;
                }
            } else {
                daylight = 1;
                if(!found_dst) {
                    strncpy(__tzname_dst, abbr, sizeof(__tzname_dst) - 1);
                    found_dst = 1;
                }
            }
        }
    }
}
