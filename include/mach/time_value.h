#ifndef MACH_TIME_VALUE_H
#define MACH_TIME_VALUE_H

#include <features.h>

#if defined(_MACH_SOURCE)

# define TIME_MICROS_MAX 1000000

# if defined(_C_SOURCE)

#  include <mach/machine/vm_types.h>

struct time_value {
    integer_t seconds;
    integer_t microseconds;
};

typedef struct time_value time_value_t;

#  define time_value_add_usec(val, micros)                                                         \
      do {                                                                                         \
          (val)->microseconds += (micros);                                                         \
          if ((val)->microseconds >= TIME_MICROS_MAX) {                                            \
              (val)->microseconds -= TIME_MICROS_MAX;                                              \
              (val)->seconds++;                                                                    \
          }                                                                                        \
      } while (0)

#  define time_value_add(result, addend)                                                           \
      do {                                                                                         \
          (result)->seconds += (addend)->seconds;                                                  \
          time_value_add_usec((result), (addend)->microseconds);                                   \
      } while (0)

# endif /* _C_SOURCE */

#endif /* _MACH_SOURCE */

#endif /* !MACH_TIME_VALUE_H */
