#ifndef _MACH_PORT_H
#define _MACH_PORT_H

#include <features.h>

#if defined(_MACH_SOURCE)

# if defined(_C_SOURCE)

#  include <mach/boolean.h>
#  include <mach/machine/vm_types.h>

/* base types */
typedef natural_t mach_port_name_t;               /* port id */
typedef mach_port_name_t *mach_port_name_array_t; /* array of port ids */
typedef mach_port_name_t mach_port_t;             /* port capability */
typedef mach_port_t *mach_port_array_t;           /* array of port capabilities */
typedef int mach_port_flavor_t;                   /* attribute flavor */
typedef natural_t mach_port_right_t;              /* port right id */
typedef natural_t mach_port_type_t;               /* port rights bitmask */
typedef mach_port_type_t *mach_port_type_array_t; /* array of port types */
typedef natural_t mach_port_urefs_t;              /* user reference count */
typedef integer_t mach_port_delta_t;              /* ref count delta */

/* port attributes */
typedef natural_t mach_port_seqno_t;      /* sequence number */
typedef natural_t mach_port_mscount_t;    /* make-send count */
typedef natural_t mach_port_msgcount_t;   /* message count */
typedef natural_t mach_port_rights_t;     /* rights count */
typedef unsigned int mach_port_srights_t; /* send rights status */
typedef integer_t *mach_port_info_t;      /* generic attributes array */

/* structs */
typedef struct mach_port_status {
    mach_port_rights_t mps_pset;       /* containing port sets */
    mach_port_seqno_t mps_seqno;       /* sequence number */
    mach_port_mscount_t mps_mscount;   /* make-send count */
    mach_port_msgcount_t mps_qlimit;   /* queue limit */
    mach_port_msgcount_t mps_msgcount; /* queued messages */
    mach_port_rights_t mps_sorights;   /* send-once rights */
    boolean_t mps_srights;             /* send rights exist? */
    boolean_t mps_pdrequest;           /* port-deleted requested? */
    boolean_t mps_nsrequest;           /* no-senders requested? */
    natural_t mps_flags;               /* status flags */
} mach_port_status_t;

typedef struct mach_port_limits {
    mach_port_msgcount_t mpl_qlimit; /* message queue limit */
} mach_port_limits_t;

typedef struct mach_port_qos {
    unsigned int name : 1;     /* specific name requested */
    unsigned int prealloc : 1; /* message preallocated */
    boolean_t pad1 : 30;       /* padding */
    natural_t len;             /* preallocated buffer size */
} mach_port_qos_t;

#  if !defined(_NO_PORT_T_FROM_MACH)
__deprecated typedef mach_port_t port_t;                  /* legacy port type */
__deprecated typedef mach_port_name_t port_name_t;        /* legacy port name */
__deprecated typedef mach_port_name_t *port_name_array_t; /* legacy names array */
#  endif

/* special port names */
#  define MACH_PORT_NULL        0                        /* null port */
#  define MACH_PORT_DEAD        ((mach_port_name_t)(~0)) /* dead port */
#  define MACH_PORT_VALID(name) (((name) + 1U) > 1U)     /* check if valid */

/* name manipulations */
#  define MACH_PORT_INDEX(name)      ((name) >> 8)                  /* get port index */
#  define MACH_PORT_GEN(name)        (((name) & 0xff) << 24)        /* get generation */
#  define MACH_PORT_MAKE(index, gen) (((index) << 8) | (gen) >> 24) /* pack index and gen */

/* port rights */
#  define MACH_PORT_RIGHT_SEND      ((mach_port_right_t)0) /* send right */
#  define MACH_PORT_RIGHT_RECEIVE   ((mach_port_right_t)1) /* receive right */
#  define MACH_PORT_RIGHT_SEND_ONCE ((mach_port_right_t)2) /* send-once right */
#  define MACH_PORT_RIGHT_PORT_SET  ((mach_port_right_t)3) /* port set right */
#  define MACH_PORT_RIGHT_DEAD_NAME ((mach_port_right_t)4) /* dead name right */
#  define MACH_PORT_RIGHT_LABELH    ((mach_port_right_t)5) /* label handle right */
#  define MACH_PORT_RIGHT_NUMBER    ((mach_port_right_t)6) /* total rights count */

/* port types */
#  define MACH_PORT_TYPE(right)                                                                    \
      ((mach_port_type_t)(((mach_port_type_t)1) << ((right) + ((mach_port_right_t)16))))

#  define MACH_PORT_TYPE_NONE      ((mach_port_type_t)0L)
#  define MACH_PORT_TYPE_SEND      MACH_PORT_TYPE(MACH_PORT_RIGHT_SEND)
#  define MACH_PORT_TYPE_RECEIVE   MACH_PORT_TYPE(MACH_PORT_RIGHT_RECEIVE)
#  define MACH_PORT_TYPE_SEND_ONCE MACH_PORT_TYPE(MACH_PORT_RIGHT_SEND_ONCE)
#  define MACH_PORT_TYPE_PORT_SET  MACH_PORT_TYPE(MACH_PORT_RIGHT_PORT_SET)
#  define MACH_PORT_TYPE_DEAD_NAME MACH_PORT_TYPE(MACH_PORT_RIGHT_DEAD_NAME)
#  define MACH_PORT_TYPE_LABELH    MACH_PORT_TYPE(MACH_PORT_RIGHT_LABELH)

#  define MACH_PORT_TYPE_SEND_RECEIVE (MACH_PORT_TYPE_SEND | MACH_PORT_TYPE_RECEIVE)
#  define MACH_PORT_TYPE_SEND_RIGHTS  (MACH_PORT_TYPE_SEND | MACH_PORT_TYPE_SEND_ONCE)
#  define MACH_PORT_TYPE_PORT_RIGHTS  (MACH_PORT_TYPE_SEND_RIGHTS | MACH_PORT_TYPE_RECEIVE)
#  define MACH_PORT_TYPE_PORT_OR_DEAD (MACH_PORT_TYPE_PORT_RIGHTS | MACH_PORT_TYPE_DEAD_NAME)
#  define MACH_PORT_TYPE_ALL_RIGHTS   (MACH_PORT_TYPE_PORT_OR_DEAD | MACH_PORT_TYPE_PORT_SET)

/* type status bits */
#  define MACH_PORT_TYPE_DNREQUEST         0x80000000 /* dead-name req registered */
#  define MACH_PORT_TYPE_SPREQUEST         0x40000000 /* send-possible req registered */
#  define MACH_PORT_TYPE_SPREQUEST_DELAYED 0x20000000 /* delayed send-possible req */

/* send rights */
#  define MACH_PORT_SRIGHTS_NONE    0 /* no send rights */
#  define MACH_PORT_SRIGHTS_PRESENT 1 /* send rights exist */

/* queue limits */
#  define MACH_PORT_QLIMIT_ZERO    ((mach_port_msgcount_t)0)
#  define MACH_PORT_QLIMIT_BASIC   ((mach_port_msgcount_t)5)
#  define MACH_PORT_QLIMIT_SMALL   ((mach_port_msgcount_t)16)
#  define MACH_PORT_QLIMIT_LARGE   ((mach_port_msgcount_t)1024)
#  define MACH_PORT_QLIMIT_KERNEL  ((mach_port_msgcount_t)65536)
#  define MACH_PORT_QLIMIT_MIN     MACH_PORT_QLIMIT_ZERO
#  define MACH_PORT_QLIMIT_DEFAULT MACH_PORT_QLIMIT_BASIC
#  define MACH_PORT_QLIMIT_MAX     MACH_PORT_QLIMIT_LARGE

#  define MACH_PORT_LIMITS_INFO         1 /* limits info flavor */
#  define MACH_PORT_RECEIVE_STATUS      2 /* receive status flavor */
#  define MACH_PORT_DNREQUESTS_SIZE     3 /* dead-name reqs size */
#  define MACH_PORT_TEMPOWNER           4 /* temp owner flavor */
#  define MACH_PORT_IMPORTANCE_RECEIVER 5 /* importance receiver flavor */

#  define MACH_PORT_LIMITS_INFO_COUNT ((natural_t)(sizeof(mach_port_limits_t) / sizeof(natural_t)))
#  define MACH_PORT_RECEIVE_STATUS_COUNT                                                           \
      ((natural_t)(sizeof(mach_port_status_t) / sizeof(natural_t)))
#  define MACH_PORT_DNREQUESTS_SIZE_COUNT 1

/* port_t definitions. XXX: deprecated at Mach 3.0 */
#  if !defined(_NO_PORT_T_FROM_MACH)
#   define PORT_NULL        ((port_t)0)
#   define PORT_DEAD        ((port_t)(~0))
#   define PORT_VALID(name) (((port_t)(name) + 1U) > 1U)
#  endif /* !_NO_PORT_T_FROM_MACH */

# endif /* _C_SOURCE */

#endif /* _MACH_SOURCE */

#endif /* !_MACH_PORT_H */
