#ifndef SKYWIND3000_ALLOCATOR_H_
#define SKYWIND3000_ALLOCATOR_H_

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

struct allocator {
	void *(*alloc)(size_t size);
	void (*free)(void *ptr);
	void *(*realloc)(void *ptr, size_t size);
};

void *internal_alloc(struct allocator *alloctor, size_t size);
void internal_free(struct allocator *alloctor, void *ptr);
void *internal_realloc(struct allocator *allocator, void *ptr, size_t size);

#ifdef __cplusplus
}
#endif

#endif // SKYWIND3000_ALLOCATOR_H_
