#include "skywind3000_vector.h"

#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

void vector_init(struct vector *v) {
  if (v == NULL) return;
  v->data = 0;
  v->size = 0;
  v->capacity = 0;
}

void vector_destory(struct vector *v) {
  if (v == 0) return;
  if (v->data) free(v->data);
  v->size = 0;
  v->capacity = 0;
}

int vector_resize(struct vector *v, size_t newsize) {
  if (newsize > v->capacity) {
    size_t capacity = v->capacity * 2;
    if (newsize > capacity) {
      capacity = sizeof(char *);
      while (newsize > capacity) capacity *= 2;
    }
    if (vector_capacity(v, capacity) != 0) {
      return -1;
    }
  }
  v->size = newsize;
  return 0;
}

/* 可能减少 */
int vector_capacity(struct vector *v, size_t newcap) {
  if (newcap == v->capacity) return 0;
  if (newcap == 0) {
    /* 防止重复释放？？ */
    if (v->capacity > 0) free(v->data);
    v->data = 0;
    v->size = 0;
    v->capacity = 0;
  } else {
    unsigned char *ptr = (unsigned char *)malloc(newcap);
    assert(ptr);
    size_t mincopy = (newcap < v->size) ? newcap : v->size;
    memcpy(ptr, v->data, mincopy);
    free(v->data);
    v->data = ptr;
    v->capacity = newcap;
    v->size = mincopy;
  }
  return 0;
}

int vector_reserve(struct vector *v, size_t newsize) {
  return vector_capacity(v, (newsize < v->size) ? v->size : newsize);
}

int vector_push(struct vector *v, const void *ptr, size_t size) {
}

size_t vector_pop(struct vector *v, void *ptr, size_t size) {
}
