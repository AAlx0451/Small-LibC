#ifndef MMAN_H
#define MMAN_H

#include <sys/types.h>
#include <stddef.h>

/* Константы защиты памяти (соответствуют Darwin/iOS) */
#define PROT_NONE   0x00
#define PROT_READ   0x01
#define PROT_WRITE  0x02
#define PROT_EXEC   0x04

/* Флаги отображения */
#define MAP_SHARED  0x0001
#define MAP_PRIVATE 0x0002
#define MAP_FIXED   0x0010
#define MAP_ANON    0x1000

/* Алиас для совместимости с кодом malloc.c */
#define MAP_ANONYMOUS MAP_ANON

#define MAP_FAILED  ((void *)-1)

/* Прототипы функций */
void *mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset);
int munmap(void *addr, size_t length);

#endif
