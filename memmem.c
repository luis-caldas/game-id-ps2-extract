#include <assert.h>
#include <stdlib.h>
#include <string.h>

void *memmem(const void *v, size_t size, const void *pat, size_t patsize) {
	const char *p;

	assert(v != NULL);
	assert(pat != NULL);

	for (p = v; size >= patsize; ++p, --size)
		if (memcmp(p, pat, patsize) == 0)
			return (void*) p;
	return NULL;
}

void *memrmem(const void *v, size_t size, const void *pat, size_t patsize) {
	const char *p;

	assert(v != NULL);
	assert(pat != NULL);

	/* Special cases */
	if (size < patsize)
		return NULL;
	if (patsize == 0)
		return (void *) v;

	/* xxx - this will make p < v if pattern not found; potential
	   portability problem */
	for (p = v, p += size-patsize; size >= patsize; --p, --size)
		if (memcmp(p, pat, patsize) == 0)
			return (void *) p;
	return NULL;
}
