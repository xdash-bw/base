#ifndef __SKYWIND3000_RBTREE_H
#define __SKYWIND3000_RBTREE_H

#include <stddef.h>

struct rb_node {
    struct rb_node *left, *right, *parent;
#define RB_RED 0
#define RB_BLACK 1
    unsigned char color;
}; 

struct rb_root {
    struct rb_node* root;
};


// -----------------------------------------------------------
// rbtree defination
// -----------------------------------------------------------
#define RB_ROOT_INIT(rb_root) ((rb_root)->root = 0)

#define RB_NODE_INIT(node) do {                             \
        (node)->left = (node)->right = (node)->parent = 0;  \
        (node)->color = RB_RED; } while (0)

#define OFFSET_OF(type, member) ((size_t) &((type*)0)->member)
#define CONTAINER_OF(ptr, type, member) ( \
        (type*) (((char*)((type*)(ptr))) - OFFSET_OF(type, member)))
    
#define RB_NODE_ENTRY(ptr, type, member) CONTAINER_OF(ptr, type, member)


// ------------------------------------------------------------
// rbtree operation
// ------------------------------------------------------------
#define RB_INSERT_NODE(rb_root, node, compare_fn, tag) do {    \
        int __ret = 0;                                      \
        struct rb_node *__tag = 0;                          \
        struct rb_node *__parent = 0;                       \
        struct rb_node **__link = &((rb_root)->root);          \
        while (__link[0]) {                                 \
            __parent = __link[0];                           \
            __ret = (compare_fn)(__link[0], (node));        \
            if (__ret == 0) {                               \
                __tag = __link[0];                          \
                break;                                      \
            } else if (__ret < 0) {                         \
                __link = &(__link[0]->left);                \
            } else if (__ret > 0) {                         \
                __link = &(__link[0]->right);               \
            }                                               \
        }                                                   \
        (tag) = __tag;                                      \
        if (__tag == 0) {                                   \
            __rb_link_node((node), __parent, __link);         \
            rb_insert_color((rb_root), (node));                \
        }                                                   \
    } while (0)

static inline void __rb_link_node(struct rb_node *newnode, 
                                struct rb_node *parent, 
                                struct rb_node **link) 
{
    newnode->parent = parent;
    newnode->left = newnode->right = 0;
    newnode->color = RB_RED;
    *link = newnode;
}

#define RB_FIND_NODE(rb_root, node, compare_fn, result) do {   \
        struct rb_node *__p = (rb_root)->root;                 \
        int __ret = 0;                                      \
        (result) = 0;                                    \
        while (__p) {                                       \
            __ret = compare_fn(__p, node);                  \
            if (__ret == 0) {                               \
                result = __p;                             \
                break;                                      \
            }                                               \
            else if (__ret < 0) {                           \
                __p = __p->left;                            \
            } else if (__ret > 0) {                         \
                __p = __p->right;                           \
            }                                               \
        }                                                   \
    } while (0)


// ------------------------------------------------------------
// rbtree interface
// ------------------------------------------------------------
#define rb_root_init    RB_ROOT_INIT
#define rb_node_init    RB_NODE_INIT
#define rb_node_entry   RB_NODE_ENTRY
#define rb_insert_node  RB_INSERT_NODE
#define rb_find_node    RB_FIND_NODE

extern struct rb_node *rb_node_first(struct rb_root *root);
extern struct rb_node *rb_node_last(struct rb_root *root);
extern struct rb_node *rb_node_next(struct rb_node *node);
extern struct rb_node *rb_node_prev(struct rb_node *node);
extern void rb_insert_color(struct rb_root *root, struct rb_node *newnode);
extern void rb_erase(struct rb_root *root, struct rb_node *node);




#endif
