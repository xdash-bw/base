#ifndef __SKYWIND3000_QUEUE_H
#define __SKYWIND3000_QUEUE_H

struct list_head {
    struct list_head *prev, *next;
};


// --------------------------------------------------------------------------
// list defination
// --------------------------------------------------------------------------
#define LIST_HEAD_INIT(name) { &(name), &(name) }
#define LIST_HEAD(name) \
    struct list_head name = LIST_HEAD_INIT(name)

#define LIST_INIT(ptr) ( \
        (ptr)->prev = (ptr), (ptr)->next = (ptr))

#define OFFSETOF(type, member) ((size_t) &((type*)0)->member)
#define CONTAINER_OF(ptr, type, member) ( \
    (type*)(((char*)((type*)(ptr))) - OFFSETOF(type, member)))

#define LIST_ENTRY(ptr, type, member) CONTAINER_OF(ptr, type, member)


// ---------------------------------------------------------------------------
// list operation 
// ---------------------------------------------------------------------------
#define LIST_ADD(node, head) ( \
        (node)->prev = (head), (node)->next = (head)->next, \
        (head)->next->prev = (node), (head)->next = (node))

#define LIST_ADD_TAIL(node, head) ( \
        (node)->prev = (head)->prev, (node)->next = (head), \
        (head)->prev->next = (node), (head)->prev = (node))

#define LIST_DEL(entry) ( \
        (entry)->prev->next = (entry)->next, \
        (entry)->next->prev = (entry)->prev, \
        (entry)->prev = (entry)->next = 0)

#define LIST_DEL_INIT(entry) do { \
        LIST_DEL(entry); LIST_INIT(entry); } while (0)

#define LIST_IS_EMPTY(entry) ((entry) == (entry)->next)

#define LIST_FOREACH(iterator, head, type, member)            \
    for ((iterator) = LIST_ENTRY((head)->next, type, member); \
         &((iterator)->member) != (head); \
         (iterator) = LIST_ENTRY((iterator)->member.next, type, member))

#define __LIST_SPLICE(head, list) do { \
        struct list_head *first = (list)->next; \
        struct list_head *last = (list)->prev; \
        struct list_head *at = (head)->next; \
        (first)->prev = (head), (last)->next = (at); \
        (head)->next = (first), (at)->prev = (last); } while (0)

#define LIST_SPLICE(head, list) do { \
        if (!LIST_IS_EMPTY(head)) __LIST_SPLICE(head, list); } while (0)

      
#endif
