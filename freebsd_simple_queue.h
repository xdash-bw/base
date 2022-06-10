#ifndef FREEBSD_SIMPLE_QUEUE_H_
#define FREEBSD_SIMPLE_QUEUE_H_

#include <assert.h>
/*
 * 模仿 freebsd 的 simple_queue 实现，节点只有个 next 的单向链表，头节点拥有
 * first 和 last 指针。 所以可以进行头和尾操作，用于实现队列
 */
#ifdef __cplusplus
extern "C" {
#endif

  /*
    simple queue definations
  */

#define SIMPLEQ_HEAD(name, type)                \
  struct name                                   \
  {                                             \
    struct type *sqh_first;                     \
    struct type **sqh_last;                     \
  }

#define SIMPLEQ_ENTRY(type)                     \
  struct                                        \
  {                                             \
    struct type *sqe_next;                      \
  }

  /*
    simple queue access methods
  */
#define SIMPLEQ_FIRST(head) ((head)->sqh_first)

#define SIMPLEQ_END(head) NULL

#define SIMPLEQ_NEXT(elm, field) ((head)->field.sqe_next)

#define SIMPLEQ_EMPTY(head) (SIMPLEQ_FIRST(head) == SIMPLEQ_END(head))

#define SIMPLEQ_FOREACH(iter, head, field)      \
  for ((iter) = SIMPLEQ_FIRST(head);            \
       (iter) != SIMPLEQ_END(head);             \
       (iter) = SIMPLEQ_NEXT(elm, field))

  /*
    simple functions
  */
#define SIMPLEQ_INIT(head) do {                 \
    (head)->sqh_first = NULL;                   \
    (head)->sqh_last = &(head)->sqh_first;      \
  } while (0)

#define SIMPLEQ_ENTRY_INIT(elm, field) do {     \
    (elm)->field.sqe_next = NULL;               \
  } while (0)

#define SIMPLEQ_INSERT_TAIL(head, elm, field) do {  \
    *((head)->sqh_last) = (elm);                    \
    (head)->sqh_last = &(elm)->field.sqe_next;      \
  } while (0)

#define SIMPLEQ_INSERT_HEAD(head, elm, field) do {        \
    if (SIMPLEQ_EMPTY(head))                              \
      {                                                   \
        (head)->field.sqh_last = &(elm)->field.sqe_next;  \
      }                                                   \
    (elm)->field.sqe_next = (head)->sqh_first;            \
    (head)->sqh_first = (elm);                            \
  } while (0)

#define SIMPLEQ_REMOVE_HEAD(head, elm, field) do {  \
    (head)->sqh_first = (elm)->field.sqe_next;      \
    if (SIMPLEQ_EMPTY((head)))                      \
      {                                             \
        (head)->sqh_last = &(head)->sqh_first;      \
      }                                             \
    (elm)->field.sqe_next = NULL;                   \
  } while (0)

#ifdef __cplusplus
}
#endif

#endif
