#ifndef FREEBSD_LIST_H_
#define FREEBSD_LIST_H_

/*
 * 仿 freebsd 的内核链表实现
 * 非常经典的链表，以 O(1) 头部插入，任意节点 O(1) 移出链表
 */

#ifdef __cplusplus
extern "C" {
#endif

  /*
   *  list definations
   */
#define LIST_HEAD(name, type)                   \
  struct name                                   \
  {                                             \
    struct type *lh_first;                      \
  }

#define LIST_ENTRY(type)                        \
  struct                                        \
  {                                             \
    struct type *le_next;                       \
    struct type **le_prev;                      \
  }

  /*
   * list access methods
   */
#define LIST_FIRST(head) ((head)->lh_first)
#define LIST_END(head) NULL
#define LIST_NEXT(elm, field) ((elm)->field.le_next)
#define LIST_EMPTY(head) (LIST_FIRST(head) == LIST_END(head))

#define LIST_FOREACH(elm, head, field)          \
  for ((elm) = LIST_FIRST(head);                \
       (elm) != LIST_END(head);                 \
       (elm) = LIST_NEXT(elm, field))

  /*
   * list functions
   */
#define LIST_INIT(head) (LIST_FIRST(head) = LIST_END(head))

#define LIST_ENTRY_INIT(entry, field) do {                              \
    (entry)->field.le_next = NULL; (entry)->field.le_prev = NULL) } while (0)

#define LIST_INSERT_HEAD(head, elm, field) do {                         \
    (elm)->field.le_next = (head)->lh_first;                            \
    (elm)->field.le_prev = &((head)->lh_first);                         \
    if (((elm)->field.le_next) != NULL)                                 \
      {                                                                 \
        (elm)->field.le_next->field.le_prev = &((elm)->field.le_next);  \
      }                                                                 \
    (head)->lh_first = (elm);                                           \
  } while (0)

#define LIST_REMOVE(elm, field) do {                                \
    *((elm)->field.le_prev) = (elm)->field.le_next;                 \
    if ((elm)->field.le_next != NULL)                               \
      {                                                             \
        (elm)->field.le_next->field.le_prev = (elm)->field.le_prev; \
      }                                                             \
  } while(0)

#ifdef __cplusplus
}
#endif

#endif
