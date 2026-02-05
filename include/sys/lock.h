#ifndef SYS_LOCK_H
#define SYS_LOCK_H

#define UNLOCK  0
#define PROCLOCK 1
#define TXTLOCK 2
#define DATLOCK 4

int plock(int);

#endif // SYS_LOCK_H
