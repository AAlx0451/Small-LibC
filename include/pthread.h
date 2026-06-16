#ifndef _PTHREAD_H
#define _PTHREAD_H

#include <features.h>

#if !defined(_ANSI) && (defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 199506L)

# include <mach/port.h>
# include <mach/semaphore.h>
# include <sched.h>
# include <stddef.h>
# include <stdint.h>

# define MAXTHREADNAMESIZE               64
# define _EXTERNAL_POSIX_THREAD_KEYS_MAX 256
# define _INTERNAL_POSIX_THREAD_KEYS_MAX 256
# define _INTERNAL_POSIX_THREAD_KEYS_END                                                           \
     (_EXTERNAL_POSIX_THREAD_KEYS_MAX + _INTERNAL_POSIX_THREAD_KEYS_MAX)

typedef int pthread_lock_t;

struct __darwin_pthread_handler_rec {
    void (*__routine)(void *arg);
    void *__arg;
    struct __darwin_pthread_handler_rec *__next;
};

struct _pthread {
    long sig;
    struct __darwin_pthread_handler_rec *__cleanup_stack;
    pthread_lock_t lock;

    uint32_t detached : 8;
    uint32_t inherit : 8;
    uint32_t policy : 8;
    uint32_t freeStackOnExit : 1;
    uint32_t newstyle : 1;
    uint32_t kernalloc : 1;
    uint32_t schedset : 1;
    uint32_t wqthread : 1;
    uint32_t wqkillset : 1;
    uint32_t pad : 2;

    size_t guardsize;
    int pad0;

    struct sched_param param;
    uint32_t cancel_error;

    struct _pthread *joiner;
    int pad1;

    void *exit_value;
    semaphore_t death;
    mach_port_t kernel_thread;

    void *(*fun)(void *arg);
    void *arg;
    int cancel_state;
    int err_no;

    void *tsd[_INTERNAL_POSIX_THREAD_KEYS_END];

    void *stackaddr;
    size_t stacksize;
    mach_port_t reply_port;

    void *cthread_self;

    uint32_t childrun : 1;
    uint32_t parentcheck : 1;
    uint32_t childexit : 1;
    uint32_t pad3 : 29;

    struct {
        struct _pthread *tqe_next;
        struct _pthread **tqe_prev;
    } plist;

    void *freeaddr;
    size_t freesize;
    mach_port_t joiner_notify;
    char pthread_name[MAXTHREADNAMESIZE];
    int max_tsd_key;
    void *cur_workq;
    void *cur_workitem;
    uint64_t thread_id;
};

typedef struct _pthread *pthread_t;

#endif /* !_ANSI && (_POSIX_C_SOURCE && _POSIX_C_SOURCE >= 199506L) */

#endif /* !_PTHREAD_H */
