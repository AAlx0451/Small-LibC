#include <stdlib.h>

void *bsearch(const void *key, const void *base, size_t nmemb, size_t size, 
              int (*compar)(const void *, const void *)) {
	const char *ptr_base = (const char *)base;
	size_t l = 0;
	size_t r = nmemb;
	size_t idx;
	const void *p;
	int cmp;

	while (l < r) {
		idx = l + (r - l) / 2;
		p = (const void *)(ptr_base + idx * size);
		cmp = compar(key, p);

		if (cmp < 0) {
			r = idx;
		} else if (cmp > 0) {
			l = idx + 1;
		} else {
			return (void *)p;
		}
	}

	return NULL;
}
