#include "skywind3000_rbtree.h"

#include <assert.h>

struct rb_node * rb_node_first(struct rb_root *root){
    assert(root);
    struct rb_node *p = root->root;
    while (p->left) p = p->left;
    return p;
}

struct rb_node * rb_node_last(struct rb_root *root) {
    assert(root);
    struct rb_node *p = root->root;
    while (p->right) p = p->right;
    return p;
}

struct rb_node * rb_node_next(struct rb_node *node){
    assert(node);
    
    if (node->right) {
        node = node->right;
        while (node->left) 
            node = node->left;
        return node;
    }
    
    if (node->parent && node->parent->left == node) 
        return node;
    
    return 0;
}

struct rb_node * rb_node_prev(struct rb_node *node){
    assert(node);
    
    if (node->left) {
        node = node->left;
        while (node->right) 
            node = node->right;
        return node;
    }
    
    if (node->parent && node->parent->right == node)
        return node->parent;
    
    return 0;
}

static void __rb_rotate_left(struct rb_node *node, struct rb_root *root) {
    struct rb_node *right = node->right;
    assert(right);
    
    if ((node->right = right->left) != 0) {
        node->right->parent = node;
    }
    
    right->left = node;
    struct rb_node *node_p = node->parent;
    node->parent = right;
    
    if ((right->parent = node_p) != 0) {
        if (right->parent->left == node) 
            right->parent->left = right;
        else 
            right->parent->right = right;
    }
    else {
        root->root = right;
    }
}    

static void __rb_rotate_right(struct rb_node *node, struct rb_root *root) {
    struct rb_node *left = node->left;
    assert(left);
    
    if ((node->left = left->right) != 0) {
        node->left->parent = node;
    }
    
    left->right = node;
    struct rb_node *node_p = node->parent;
    node->parent = left;
    
    if ((left->parent = node_p) != 0) {
        if (left->parent->left == node) 
            left->parent->left = left;
        else 
            left->parent->right = left;
    } else {
        root->root = left;
    }
}
    
void rb_insert_color(struct rb_root *root, struct rb_node *node) {
    struct rb_node *parent;
    struct rb_node *gparent;

    while ((parent = node->parent) && parent->color == RB_RED) {
        gparent = parent->parent;
        assert(gparent);

        if (parent == gparent->left) {
            register struct rb_node *uncle;
            if ((uncle = gparent->right) && uncle->color == RB_RED) {
                parent->color = RB_BLACK;
                uncle->color = RB_BLACK;
                gparent->color = RB_RED;
                node = gparent;
                continue;
            }

            if (node == parent->right) {
                register struct rb_node *tmp;
                __rb_rotate_left(parent, root);
                tmp = node;
                node = parent;
                parent = tmp;
            }

            parent->color = RB_BLACK;
            gparent->color = RB_RED;
            __rb_rotate_right(gparent, root);
        }
        else {
            register struct rb_node *uncle;
            if ((uncle = gparent->left) && uncle->color == RB_RED) {
                parent->color = RB_BLACK;
                uncle->color = RB_BLACK;
                gparent->color = RB_RED;
                node = gparent;
                continue;
            }

            if (node == parent->left) {
                register struct rb_node *tmp;
                __rb_rotate_right(parent, root);
                tmp = node;
                node = parent;
                parent = tmp;
            }

            parent->color = RB_BLACK;
            gparent->color = RB_RED;
            __rb_rotate_left(gparent, root);
        }
    }
    root->root->color = RB_BLACK;
}

static void __rb_erase_color(struct rb_node *node, struct rb_node *parent, struct rb_root *root) {
    struct rb_node *other;

    while ((!node || node->color == RB_BLACK) && node != root->root) {
        if (parent->left == node) {
            other = parent->right;
            if (other->color == RB_RED) {
                other->color = RB_BLACK;
                parent->color = RB_RED;
                __rb_rotate_left(parent, root);
                other = parent->right;
            }
            if ((!other->left || other->left->color == RB_BLACK) && (!other->right || other->right->color == RB_BLACK)) {
                other->color = RB_RED;
                node = parent;
                parent = node->parent;
            }
            else {
                if (!other->right || other->right->color == RB_BLACK) {
                    register struct rb_node *o_left;
                    if ((o_left = other->left)) o_left->color = RB_BLACK;
                    other->color = RB_RED;
                    __rb_rotate_right(other, root);
                    other = parent->right;
                }
                other->color = parent->color;
                parent->color = RB_BLACK;
                if (other->right) other->right->color = RB_BLACK;
                __rb_rotate_left(parent, root);
                node = root->root;
                break;
            }
        }
        else {
            other = parent->left;
            if (other->color == RB_RED) {
                other->color = RB_BLACK;
                parent->color = RB_RED;
                __rb_rotate_right(parent, root);
                other = parent->left;
            }
            if ((!other->left || other->left->color == RB_BLACK) && (!other->right || other->right->color == RB_BLACK)) {
                other->color = RB_RED;
                node = parent;
                parent = node->parent;
            }
            else {
                if (!other->left || other->left->color == RB_BLACK) {
                    register struct rb_node *o_right;
                    if ((o_right = other->right)) o_right->color = RB_BLACK;
                    other->color = RB_RED;
                    __rb_rotate_left(other, root);
                    other = parent->left;
                }
                other->color = parent->color;
                parent->color = RB_BLACK;
                if (other->left) other->left->color = RB_BLACK;
                __rb_rotate_right(parent, root);
                node = root->root;
                break;
            }
        }
    }
    if (node) node->color = RB_BLACK;
}

void rb_erase(struct rb_root *root, struct rb_node *node) {
    struct rb_node *parent;
    struct rb_node *child;
    int color;

    /* 如果待删除节点 node 只有一个子节点，则获取其子节点，链接上层父节点即可 */
    if (node->left == 0) {
        child = node->right;
    }
    else if (node->right == 0) {
        child = node->left;
    }

    /* 如果两个节点都存在 */
    /* 这是最麻烦的情况, 要确保其左右子树平衡 */
    /* 选择其右边子树的最小（最左）节点顶替待删除节点 node 的位置 */
    else {
        struct rb_node *left;
        struct rb_node *old = node;
        /* 从右子树开始遍历 */
        node = node->right;
        /* 直到 node 是右子树最小（最左）节点 */
        while ((left = node->left) != 0) {
            node = left;
        }

        color = node->color;
        child = node->right;
        parent = node->parent;

        if (child) {
            child->parent = parent;
        }
        if (parent->left == node) {
            parent->left = child;
        }
        else {
            parent->right = child;
        }

        if (parent == old) {
            parent = node;
        }

        node->parent = old->parent;
        node->left = old->left;
        node->right = old->right;
        node->color = old->color;

        if (node->parent) {
            if (node->parent->left == old) {
                node->parent->left = node;
            }
            else  {
                node->parent->right = node;
            }
        }
        else {
            root->root = node;
        }

        node->left->parent = node;
        if (node->right) {
            node->right->parent = node;
        }
        goto color;
    }

    parent = node->parent;
    color = node->color;

    if (child) {
        child->parent = parent;
    }
    if (parent) {
        if (parent->left == node) {
            parent->left = child;
        }
        else {
            parent->right = child;
        }
    }
    else {
        root->root = child;
    }

color:
    if (color == RB_BLACK) {
        __rb_erase_color(child, parent, root);
    }
}
    

