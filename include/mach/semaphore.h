#ifndef MACH_SEMAPHORE_H
#define MACH_SEMAPHORE_H

#include <mach/kern_return.h>
#include <mach/mach_types.h>
#include <mach/port.h>
#include <mach/sync_policy.h>

kern_return_t semaphore_signal(semaphore_t semaphore);
kern_return_t semaphore_signal_all(semaphore_t semaphore);
kern_return_t semaphore_wait(semaphore_t semaphore);
kern_return_t semaphore_timedwait(semaphore_t semaphore, mach_timespec_t wait_time);
kern_return_t semaphore_signal_thread(semaphore_t semaphore, thread_t thread);

kern_return_t semaphore_timedwait_signal(semaphore_t wait_semaphore,
                                         semaphore_t signal_semaphore,
                                         mach_timespec_t wait_time);

kern_return_t semaphore_wait_signal(semaphore_t wait_semaphore, semaphore_t signal_semaphore);

#endif /* !MACH_SEMAPHORE_H */
