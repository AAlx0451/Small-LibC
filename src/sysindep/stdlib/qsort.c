#include <stddef.h>
#include <stdlib.h>

static inline void swap(void *a, void *b, size_t size) {
    char *p = (char *)a;
    char *q = (char *)b;
    for(size_t i = 0; i < size; i++) {
        char temp = p[i];
        p[i] = q[i];
        q[i] = temp;
    }
}

static inline void qsort_recursive(void *base, size_t nmemb, size_t size,
                                   int (*compar)(const void *, const void *)) {
    char *arr;
    size_t i = 0, j;
    void *pivot;
    if(nmemb <= 1) {
        return;
    }
    arr = (char *)base;
    pivot = arr + (nmemb - 1) * size;
    for(j = 0; j < nmemb - 1; j++) {
        if(compar(arr + j * size, pivot) < 0) {
            swap(arr + i * size, arr + j * size, size);
            i++;
        }
    }
    swap(arr + i * size, pivot, size);
    qsort_recursive(arr, i, size, compar);
    qsort_recursive(arr + (i + 1) * size, nmemb - i - 1, size, compar);
}

void qsort(void *base, size_t nmemb, size_t size,
           int (*compar)(const void *, const void *)) {
    if(base == NULL || nmemb == 0 || size == 0 || compar == NULL) {
        return;
    }
    qsort_recursive(base, nmemb, size, compar);
}
