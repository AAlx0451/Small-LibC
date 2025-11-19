#include <stdlib.h>

static void _qsort_swap(char *a, char *b, size_t size) {
	char t;
	while (size--) {
		t = *a;
		*a++ = *b;
		*b++ = t;
	}
}

void qsort(void *base, size_t nmemb, size_t size, int (*compar)(const void *, const void *)) {
	char *ptr = (char *)base;
	char *l, *r, *p;
	size_t p_idx;
	
	if (nmemb < 2) {
		return;
	}

	p_idx = nmemb / 2;
	p = ptr + p_idx * size;
	_qsort_swap(p, ptr + (nmemb - 1) * size, size);
	
	p = ptr + (nmemb - 1) * size;
	l = ptr;
	r = ptr + (nmemb - 2) * size;
	while (l <= r) {
		if (compar(l, p) <= 0) {
			l += size;
		} else {
			_qsort_swap(l, r, size);
			if (r == ptr) break;
			r -= size;
		}
	}
	_qsort_swap(l, p, size);

	qsort(base, (l - ptr) / size, size, compar);
	qsort(l + size, nmemb - 1 - (l - ptr) / size, size, compar);
}
