#ifndef FREEBSD_TAIL_QUEUE_H_
#define FREEBSD_TAIL_QUEUE_H_

/*
 * 仿 freebsd 內核的 tail Queue 实现。
 * freebsd 的 list 升级版，头节点添加了 **last
 * 指针。因此不同于普通链表只能进行头插，它还可以进行基于尾部进行操作。所以它可以做普通链表，也可以做双端队列和栈。
 */

/*
 *  Tailq defination
 */
#define TAILQ_HEAD(name, type)					\
  struct name {									\
	struct type *tqh_first;						\
	struct type **tqh_last;						\
  }

#define TAILQ_ENTRY(type)						\
  struct {										\
	struct type *tqe_next;						\
	struct type **tqe_prev;						\
  }

/*
 * Tailq access methods
 */
#define TAILQ_FIRST(head) ((head)->tqh_first)
#define TAILQ_END(head) NULL
#define TAILQ_NEXT(elm, field) ((elm)->field.tqe_next)
#define TAILQ_EMPTY(head) (TAILQ_FIRST(head) == TAILQ_END(head))
/* 这个宏太厉害了，真的给跪了 */
#define TAILQ_LAST(head, headname)							\
  (*(((struct headname *)((head)->tqh_last))->tqh_last))

#define TAILQ_FOREACH(elm, head, field)						\
  for ((elm) = TAILQ_FIRST(head); (elm) != TAILQ_END(head);	\
	   (elm) = TAILQ_NEXT(elm, field))

/*
 * Tailq functions
 */
#define TAILQ_INIT(head)						\
  do {											\
	(head)->tqh_first = NULL;					\
	(head)->tqh_last = &((head)->tqh_first);	\
  } while (0)

#define TAILQ_INSERT_HEAD(head, elm, field)						\
  do {															\
	(elm)->field.tqe_prev = &(head)->tqh_first;					\
	if (((elm)->field.tqe_next = (head)->tqh_first) != NULL) {	\
	  (head)->tqh_first->field.tqe_prev =						\
		&((elm)->field.tqe_next);								\
	} else {													\
	  (head)->tqh_last = &((elm)->field.tqe_next);				\
	}															\
	(head)->tqh_first = (elm);									\
  } while (0)

#define TAILQ_INSERT_TAIL(head, elm, field)		\
  do {											\
  } while (0)

#endif
