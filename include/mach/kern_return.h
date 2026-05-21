#ifndef MACH_KERN_RETURN_H
#define MACH_KERN_RETURN_H

#include <features.h>
#if defined(_MACH_SOURCE)

#include <mach/machine/kern_return.h>

#define KERN_SUCCESS 0 /* operation success */
#define KERN_INVALID_ADDRESS 1 /* address is invalid */
#define KERN_PROTECTION_FAILURE 2 /* address is valid, but does not allow operation */
#define KERN_NO_SPACE 3 /* address is in use, or has incorrect size */
#define KERN_INVALID_ARGUMENT 4 /* invalid function argument */
#define KERN_FAILURE 5 /* function could not be done */
#define KERN_RESOURCE_SHORTAGE 6 /* not enough system resource to perform request */
#define KERN_NOT_RECEIVER 7 /* task does not hold receive rights for port */
#define KERN_NO_ACCESS 8 /* bogus access restriction */
#define KERN_MEMORY_FAILURE 9 /* referenced memory object was destroyed */
#define KERN_MEMORY_ERROR 10 /* memory object could not return data */
#define KERN_ALREADY_IN_SET 11 /* receive right is already in port set */
#define KERN_NOT_IN_SET 12 /* receive right is not in port set */
#define KERN_NAME_EXISTS 13 /* name already denotes a right in task */
#define KERN_ABORTED 14 /* operation was aborted */
#define KERN_INVALID_NAME 15 /* name does not denote a right in task */
#define KERN_INVALID_TASK 16 /* target task is inactive */
#define KERN_INVALID_RIGHT 17 /* invalid right for this name */
#define KERN_INVALID_VALUE 18 /* value out of range */
#define KERN_UREFS_OVERFLOW 19 /* user reference limit overflow */
#define KERN_INVALID_CAPABILITY 20 /* invalid port capability */
#define KERN_RIGHT_EXISTS 21 /* rights already exist under another name */
#define KERN_INVALID_HOST 22 /* target host is invalid */
#define KERN_MEMORY_PRESENT 23 /* memory is already present in memory object */
#define KERN_MEMORY_DATA_MOVED 24 /* page pushed into a copy of memory object */
#define KERN_MEMORY_RESTART_COPY 25 /* restart copy with quicker method */
#define KERN_INVALID_PROCESSOR_SET 26 /* invalid processor set control port */
#define KERN_POLICY_LIMIT 27 /* scheduling attributes exceed limits */
#define KERN_INVALID_POLICY 28 /* scheduling policy not enabled for processor set */
#define KERN_INVALID_OBJECT 29 /* failed to initialize memory object */
#define KERN_ALREADY_WAITING 30 /* another thread is already waiting for event */
#define KERN_DEFAULT_SET 31 /* cannot destroy default processor set */
#define KERN_EXCEPTION_PROTECTED 32 /* exception port is protected */
#define KERN_INVALID_LEDGER 33 /* ledger was not supplied */
#define KERN_INVALID_MEMORY_CONTROL 34 /* invalid memory cache control port */
#define KERN_INVALID_SECURITY 35 /* invalid host security port */
#define KERN_NOT_DEPRESSED 36 /* thread is not depressed */
#define KERN_TERMINATED 37 /* object has been terminated */
#define KERN_LOCK_SET_DESTROYED 38 /* lock set has been destroyed */
#define KERN_LOCK_UNSTABLE 39 /* lock owner terminated before release */
#define KERN_LOCK_OWNED 40 /* lock is owned by another thread */
#define KERN_LOCK_OWNED_SELF 41 /* lock is already owned by self */
#define KERN_SEMAPHORE_DESTROYED 42 /* semaphore has been destroyed */
#define KERN_RPC_SERVER_TERMINATED 43 /* RPC server terminated before reply */
#define KERN_RPC_TERMINATE_ORPHAN 44 /* terminate orphaned RPC activation */
#define KERN_RPC_CONTINUE_ORPHAN 45 /* allow orphaned RPC activation to continue */
#define KERN_NOT_SUPPORTED 46 /* empty thread activation */
#define KERN_NODE_DOWN 47 /* remote node down or inaccessible */
#define KERN_NOT_WAITING 48 /* signalled thread was not waiting */
#define KERN_OPERATION_TIMED_OUT 49 /* operation timed out */
#define KERN_CODESIGN_ERROR 50 /* page fault due to signature check */

#define KERN_RETURN_MAX 0x100 /* max kern return value */

#endif /* _MACH_SOURCE */

#endif /* !MACH_KERN_RETURN_H */
