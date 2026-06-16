#ifndef _MACH_CLOCK_TYPES_H
#define _MACH_CLOCK_TYPES_H

#include <features.h>

#if defined(_MACH_SOURCE)

# include <mach/time_value.h>

# define SYSTEM_CLOCK   0
# define CALENDAR_CLOCK 1
# define REALTIME_CLOCK 0

# define CLOCK_GET_TIME_RES 1
# define CLOCK_ALARM_CURRES 3
# define CLOCK_ALARM_MINRES 4
# define CLOCK_ALARM_MAXRES 5

# define NSEC_PER_USEC 1000
# define USEC_PER_SEC  1000000
# define NSEC_PER_SEC  1000000000
# define NSEC_PER_MSEC 1000000ull

# define ALRMTYPE      0xff
# define TIME_ABSOLUTE 0x00
# define TIME_RELATIVE 0x01

# if defined(_C_SOURCE)

typedef int alarm_type_t;
typedef int sleep_type_t;
typedef int clock_id_t;
typedef int clock_flavor_t;
typedef int *clock_attr_t;
typedef int clock_res_t;

struct mach_timespec {
    unsigned int tv_sec;
    clock_res_t tv_nsec;
};
typedef struct mach_timespec mach_timespec_t;

#  define BAD_MACH_TIMESPEC(ts)                                                                    \
      (((ts)->tv_nsec < 0) || ((ts)->tv_nsec >= (clock_res_t)NSEC_PER_SEC))

#  define CMP_MACH_TIMESPEC(ts1, ts2)                                                              \
      (((ts1)->tv_sec > (ts2)->tv_sec)   ? (int)NSEC_PER_SEC                                       \
       : ((ts1)->tv_sec < (ts2)->tv_sec) ? -(int)NSEC_PER_SEC                                      \
                                         : (int)((ts1)->tv_nsec - (ts2)->tv_nsec))

#  define ADD_MACH_TIMESPEC(ts1, ts2)                                                              \
      do {                                                                                         \
          (ts1)->tv_sec += (ts2)->tv_sec;                                                          \
          (ts1)->tv_nsec += (ts2)->tv_nsec;                                                        \
          if ((ts1)->tv_nsec >= (clock_res_t)NSEC_PER_SEC) {                                       \
              (ts1)->tv_nsec -= (clock_res_t)NSEC_PER_SEC;                                         \
              (ts1)->tv_sec += 1;                                                                  \
          }                                                                                        \
      } while (0)

#  define SUB_MACH_TIMESPEC(ts1, ts2)                                                              \
      do {                                                                                         \
          (ts1)->tv_sec -= (ts2)->tv_sec;                                                          \
          (ts1)->tv_nsec -= (ts2)->tv_nsec;                                                        \
          if ((ts1)->tv_nsec < 0) {                                                                \
              (ts1)->tv_nsec += (clock_res_t)NSEC_PER_SEC;                                         \
              (ts1)->tv_sec -= 1;                                                                  \
          }                                                                                        \
      } while (0)

#  define BAD_ALRMTYPE(t) (((t) & ~((alarm_type_t)TIME_RELATIVE)) != 0)

# endif /* _C_SOURCE */

#endif /* _MACH_SOURCE */

#endif /* !MACH_CLOCK_TYPES */
