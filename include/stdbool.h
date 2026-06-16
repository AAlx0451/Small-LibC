#ifndef _STDBOOL_H
#define _STDBOOL_H

#include <features.h>

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L

#if __STDC_VERSION__ < 202311L /* deprecated! */

#define bool _Bool
#define true 1
#define false 0

#endif /* __STDC_VERSION__ < 202311L */

#define __bool_true_false_are_defined 1

#endif /* __STDC_VERSION__ >= 199901L */

#endif /* !_STDBOOL_H */
