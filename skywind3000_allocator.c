#include "skywind3000_allocator.h"

#include <stddef.h>
#include <stdlib.h>

void *(*__hook_alloc)(size_t size) = NULL;
void (*__hook_free)(void *ptr) = NULL;
void *(*__hook_realloc)(void *ptr, size_t) = NULL;

void *internal_alloc(struct allocator *alloctor, size_t size)
{
	if (alloctor != NULL) {
		return alloctor->alloc(size);
	}
	if (__hook_alloc != NULL) {
		return __hook_alloc(size);
	}
	return malloc(size);
}

void internal_free(struct allocator *alloctor, void *ptr)
{
	if (alloctor != NULL) {
		alloctor->free(ptr);
	}
	if (__hook_free != NULL) {
		__hook_free(ptr);
	}
	free(ptr);
}

void *internal_realloc(struct allocator *allocator, void *ptr, size_t size)
{
	if (allocator != NULL) {
		return allocator->realloc(ptr, size);
	}
	if (__hook_realloc != NULL) {
		__hook_realloc(ptr, size);
	}
	return realloc(ptr, size);
}
