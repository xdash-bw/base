#ifndef __SKYWIND3000_PAGE_STREAM_H
#define __SKYWIND3000_PAGE_STREAM_H

#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include <cstdlib>

#ifdef __cplusplus
extern "C" {
#endif

struct list_head {
  struct list_head *prev, *next;
};

#define list_init(ptr) ((ptr)->prev = (ptr), (ptr)->next = (ptr))

#define list_add(node, head)                           \
  ((node)->prev = (head), (node)->next = (head)->next, \
   (head)->next->prev = (node), (head)->next = (node))

#define list_add_tail(node, head)                      \
  ((node)->prev = (head)->prev, (node)->next = (head), \
   (head)->prev->next = (node), (head)->prev = (node))

#define list_del(entry)                                                      \
  ((entry)->prev->next = (entry)->next, (entry)->next->prev = (entry)->prev, \
   (entry)->prev = (entry)->next = 0)

#define list_del_init(entry) \
  do {                       \
    list_del(entry);         \
    list_init(entry);        \
  } while (0)

#define list_entry(ptr, type, member) \
  ((type *)((char *)(((type *)(ptr))) - offsetof(type, member)))

#define list_is_empty(entry) ((entry) == (entry)->next)

#define PAGE_CACHE_SIZE 2

struct page_stream {
  struct list_head head;
  struct list_head lru;
  size_t lrusize;
  size_t size;
  size_t pos_read;
  size_t pos_write;
};

struct page {
  struct list_head head;
  size_t size;
  unsigned char data[2];
};

static inline struct page *page_new(struct page_stream *s) {
  struct page *page;
  size_t newsize;

  newsize = s->size + sizeof(struct page);

  page = (struct page *)malloc(newsize);
  if (page == NULL) return NULL;
  page->size = newsize - sizeof(struct page);

  list_init(&page->head);

  return page;
}

static inline void page_del(struct page_stream *s, struct page *page) {
  free(page);
}

static inline struct page *page_cache_get(struct page_stream *s) {
  struct page *page;

  if (s->lrusize == 0) {
    for (int i = 0; i < PAGE_CACHE_SIZE; i++) {
      page = page_new(s);
      assert(page);
      list_add_tail(&page->head, &s->lru);
      s->lrusize++;
    }
  }
  page = list_entry(&s->lru.next, struct page, head);
  list_del(&page->head);
  s->lrusize--;
  return page;
}

static inline void page_cache_release(struct page_stream *s,
                                      struct page *page) {
  list_add_tail(&page->head, &s->lru);
  s->lrusize++;

  while (s->lrusize > (PAGE_CACHE_SIZE << 1)) {
    page = list_entry(&s->lru.next, struct page, head);
    list_del(&page->head);
    s->lrusize--;
    page_del(s, page);
  }
}

static inline size_t page_stream_read_sub(struct page_stream *s, void *ptr,
                                          size_t size, int nodrop) {
  size_t total, toread, canread;

  unsigned char *lptr;
  struct page *current;
  struct list_head *head;
  size_t posread;

  lptr = (unsigned char *)ptr;
  posread = s->pos_read;
  head = s->head.next;

  for (total = 0; size > 0; size -= toread, total += toread) {
    if (head == &s->head) break;
    current = list_entry(head, struct page, head);
    head = head->next;
    if (head == &s->head) {
      canread = s->pos_write - posread;
    } else {
      canread = current->size - posread;
    }
    toread = (canread > size) ? size : canread;
    if (toread == 0) break;
    if (ptr) {
      memcpy(ptr, current->data + posread, toread);
      lptr += toread;
    }
    posread += toread;
    if (posread >= current->size) {
      posread = 0;
      if (nodrop == 0) {
        list_del(&current->head);
        page_cache_release(s, current);
        if (list_is_empty(&s->head)) {
          s->pos_write = 0;
        }
      }
    }

    if (nodrop == 0) {
      s->pos_read = posread;
    }
  }

  return total;
}

static inline size_t page_stream_write(struct page_stream *s, const void *ptr,
                                       size_t size) {
  size_t total, towrite, canwrite;
  unsigned char *lptr;
  struct page *current;

  lptr = (unsigned char *)ptr;

  for (total = 0; size > 0; total += towrite, size -= towrite) {
    if (list_is_empty(&s->head)) {
      canwrite = 0;
      current = NULL;
    } else {
      current = list_entry(&s->head.prev, struct page, head);
      canwrite = current->size - s->pos_read;
    }

    if (canwrite == 0) {
      current = page_cache_get(s);
      assert(current);
      list_add_tail(&current->head, &s->head);
      s->pos_write = 0;
      canwrite = current->size;
    }

    towrite = (canwrite > size) ? size : canwrite;
    memcpy(current->data + s->pos_write, lptr, towrite);

    s->pos_write += towrite;
    lptr += towrite;
  }

  return total;
}

#ifdef __cplusplus
}
#endif

#endif
