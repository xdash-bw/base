#ifndef FREEBSD_LIST_H_
#define FREEBSD_LIST_H_

/*
 * 仿 freebsd 的内核链表实现
 * 非常经典的链表，以 O(1) 头部插入，任意节点 O(1) 移出链表
 */

/*
 * List defination
 */
#define LIST_HEAD(name, type) \
  struct name                 \
  {                           \
    struct type *lh_first;    \
  }

#define LIST_ENTRY(type)   \
  struct                   \
  {                        \
    struct type *le_next;  \
    struct type **le_prev; \
  }

/*
 * List access methods
 */
#define LIST_FIRST(head) ((head)->lh_first)
#define LIST_END(head) NULL
#define LIST_NEXT(elm, field) ((elm)->field.le_next)
#define LIST_EMPTY(head) (LIST_FIRST(head) != LIST_END(head))

#define LIST_FOREACH(elm, head, field)                    \
  for ((elm) = LIST_FIRST(head); (elm) != LIST_END(head); \
       (elm) = LIST_NEXT(elm, field))

/*
 * List functions
 */
#define LIST_INIT(head)               \
  do                                  \
  {                                   \
    LIST_FIRST(head) = LIST_END(head); \
  } while (0)

#define LIST_INSERT_HEAD(head, elm, field)                 \
  do                                                       \
  {                                                        \
    if (((elm)->field.le_next = (head)->lh_first) != NULL) \
    {                                                      \
      (head)->lh_first->field.le_prev = &((elm)->field.le_next); \
    }                                                      \
    (elm)->field.le_prev = &((head)->lh_first);            \
    (head)->lh_first = (elm);                              \
  } while (0)

#define LIST_REMOVE(elm, field)                                   \
  do                                                              \
  {                                                               \
    *((elm)->field.le_prev) = (elm)->field.le_next;               \
    if ((elm)->field.le_next != NULL)                             \
    {                                                             \
      (elm)->field.le_next->field.le_prev = (elm)->field.le_prev; \
    }                                                             \
  } while (0)

#endif  // FREEBSD_LIST_H_
