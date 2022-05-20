#ifndef FREEBSD_LIST_H_
#define FREEBSD_LIST_H_

/*
 * 仿 freebsd
 * 的单向链表实现，最普通单向链表的升级版，每个节点除了存有下个节点地址外，存有上一个节点的“地址的地址”。
 * 任何节点可以 O(1) 移出链表。
 * O(1) 插入方面： (1) 头部插入 * (2) 基于任何节点的前面插入 (3)
 * 基于任何节点的后面插入
 */

/*
 * List defination
 */
#define LIST_HEAD(name, type) \
  struct name                 \
  {                           \
    struct type *lh_first;    \
  }

// 声明链表节点，侵入式，侵入到具体的数据结构中
#define LIST_ENTRY(type)   \
  struct                   \
  {                        \
    struct type *le_next;  \
    struct type **le_prev; \
  }

/*
 * List access methods
 */
#define LIST_BEGIN(head) ((head)->lh_first)
#define LIST_END(head) NULL
#define LIST_NEXT(elm, field) ((elm)->field.le_next)
#define LIST_EMPTY(head) (LIST_BEGIN(head) == LIST_END(head))

#define LIST_FOREACH(var, head, field) \
  for ((var) = LIST_BEGIN(var); (var) != LIST_END(head); LIST_NEXT(var, field))

/*
 * List functions
 */
#define LIST_INIT(head) (LIST_BEGIN(head) = LIST_END(head))

#define LIST_INSERT_HEAD(head, elm, field)                           \
  do                                                                 \
  {                                                                  \
    (elm)->field.le_next = head->lh_first;                           \
    (elm)->field.le_prev = &(head)->lh_first;                        \
    if ((elm)->field.le_next != NULL)                                \
    {                                                                \
      (elm)->filed.le_next->field.le_prev = &((elm)->field.le_next); \
    }                                                                \
    (head)->lh_first = (elm);                                        \
  } while (0)

#define LIST_INSERT_AFTER(listelm, elm, field)                           \
  do                                                                     \
  {                                                                      \
    if (((elm)->field.le_next = (listelm)->field.le_next) != NULL)       \
    {                                                                    \
      (listelm)->field.le_next->field.le_prev = &((elm)->field.le_next); \
    }                                                                    \
    (listelm)->field.le_next = (elm);                                    \
    (elm)->field.le_prev = &((listelm)->field.le_next);                  \
  } while (0)

#define LIST_INSERT_BEFORE(listelm, elm, field)         \
  do                                                    \
  {                                                     \
    (elm)->field.le_next = (listelm);                   \
    (elm)->field.le_prev = (listelm)->field.le_prev;    \
    *((listelm)->field.le_prev) = (elm);                \
    (listelm)->field.le_prev = &((elm)->field.le_next); \
  } while (0)

#define LIST_REMOVE(elm, field)                                   \
  do                                                              \
  {                                                               \
    if ((*((elm)->field.le_prev) = (elm)->field.le_next) != NULL) \
    {                                                             \
      (elm)->field.le_next->field.le_prev = (elm)->field.le_prev; \
    }                                                             \
  } while (0)

#define LIST_REPLACE(elm, newelm, field)                                \
  do                                                                    \
  {                                                                     \
    if (((newelm)->field.le_next = (elm)->field.le_next) != NULL)       \
    {                                                                   \
      (elm)->field.le_next->field.le_prev = &((newelm)->field.le_next); \
    }                                                                   \
    (newelm)->field.le_prev = (elm)->field.le_prev;                     \
    *((newelm)->field.le_prev) = (newelm);                              \
  } while (0)

#endif  // FREEBSD_LIST_H_
