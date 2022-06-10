#ifndef SKYWIND3000_VECTOR_H_
#define SKYWIND3000_VECTOR_H_

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

struct vector {
  unsigned char *data;
  size_t size;
  size_t capacity;
};

void vector_init(struct vector *v);
void vector_destory(struct vector *v);

int vector_resize(struct vector *v, size_t newsize);
int vector_reserve(struct vector *v, size_t newsize);
int vector_capacity(struct vector *v, size_t newcap);

int vector_push(struct vector *v, const void *ptr, size_t size);
size_t vector_pop(struct vector *v, void *ptr, size_t size);

#define vector_size(v) ((v)->size)
#define vector_data(v) ((v)->data)
#define vector_entry(v, type) ((type *)vector_data(v))

#define vector_obj_index(v, type, index) (((type *)((v)->data))[index])

#define vector_obj_push(v, type, objptr) vector_push(v, objptr, sizeof(type))
#define vector_obj_pop(v, type, objptr) vector_pop(v, objptr, sizeof(type))

#define vector_obj_resize(v, type, newsize) \
  vector_resize(v, sizeof(type) * newsize)
#define vector_obj_reserve(v, type, newsize) \
  vector_reserve(v, sizeof(type) * newsize)

#define vector_obj_size(v, type) (((v)->size) / sizeof(type))
#define vector_obj_capacity(v, type) (((v)->capacity) / sizeof(type))

#ifdef __cplusplus
}
#endif

#endif  // SKYWIND3000_VECTOR_H_
