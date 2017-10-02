#include <stdlib.h>
#include <stdbool.h>

#include "rbtree.h"


struct rbtree *alloc_rbtree(enum color c, struct rbtree *left, struct rbtree *right, int key, int value)
{
    struct rbtree *t;
    if ((t = (struct rbtree *)malloc(sizeof(struct rbtree))) == NULL)
        return NULL;
    t->color = c;
    t->left = left;
    t->right = right;
    t->key = key;
    t->value = value;
    return t;
}

void free_rbtree(struct rbtree *t)
{
    if (t == NULL)
        return;
    free_rbtree(t->left);
    free_rbtree(t->right);
    free(t);
}

int rbtree_lookup(struct rbtree *t, int key)
{
    while (t) {
        if (key < t->key)
            t = t->left;
        else if (key > t->key)
            t = t->right;
        else
            return t->value;
    }
    return -1;
}

int rbtree_valid_coloring_recur(struct rbtree *t, struct rbtree *parent)
{
    int left, right;
    if (t == NULL)
        return 1;
    if (parent == NULL && t->color != black)
        return -1;
    left = rbtree_valid_coloring_recur(t->left, t);
    right = rbtree_valid_coloring_recur(t->right, t);
    if (left < 0 || right < 0)
        return -1;
    if (left != right)
        return -1;
    return t->color == black ? left+1 : left;
}

bool rbtree_valid_coloring(struct rbtree *t)
{
    return rbtree_valid_coloring_recur(t, NULL) != -1;
}

bool rbtree_equal(struct rbtree *t1, struct rbtree *t2)
{
    if (t1 || t2) {
        return !t1 || !t2 ? false : true;
    }
    if (t1->key != t2->key || t2->value != t2->value)
        return false;
    return rbtree_equal(t1->left, t2->left) && rbtree_equal(t1->right, t2->right);
}

#define rotatable(t) ((t) && (t->left) && (t->right))

/** Rotate the red-black tree about the given node
    When this is completed, t->left should be the new root.
    @param t the pivot on which the tree is to be rotated
    @return the value of new root
*/
struct rbtree *rbtree_rightrot(struct rbtree *t)
{
    if (!rotatable(t))
        return t;
    struct rbtree *new_pivot = t->left;
    t->left = new_pivot->right;
    new_pivot->right = t;
    return new_pivot;
}

/** Rotate the red-black tree to the left, about the given node
    When this is completed t->right should be the new root
    @param t the pivot on which the tree is to be rotated
    @return the value of the new root
*/
struct rbtree *rbtree_leftrot(struct rbtree *t)
{
    if (!rotatable(t))
        return t;
    struct rbtree *new_pivot = t->right;
    t->right = new_pivot->left;
    new_pivot->left = t;
    return new_pivot;
}

/** Recursively insert node x into tree t
    In this implementation, when x is inserted, t will be the
    grandparent of x, unless t is null.
*/
struct rbtree *insert_recur(struct rbtree *t, struct rbtree *x) {
    struct rbtree *u = NULL, *p, *s;
    enum color tmp;
    if (t == NULL) // base case when dealing with a (totally) empty tree
        return x;
    // this part is a fairly standard recursive BST insertion algorithm,
    // except we insert one level down.
    if (x->key <= t->key) {
        p = t->left;
        if (p == NULL) { // this will be caught on all cases except the case where the root's children are null
            t->left = x;
            return t;
        }
        if (p->left == NULL) { // stop recursion
            p->left = x;
        } else {
            p->left = insert_recur(t->left, x);
        }
        u = t->right;
    } else { // x->key > t->key
        p = t->right;
        if (p == NULL) {
            t->right = x;
            return t;
        }
        if (p->right == NULL) {
            p->right = x;
        } else {
            p->right = insert_recur(t->right, x);
        }
        u = t->left;
    }
    if (p->color == black) { // can't have a conflict between p and x
        return t;
    }
    // p->color must be red
    // check for a color conflict
    if (p->right && p->right->color == red) {
        x = p->right;
    } else if (p->left && p->left->color == red) {
        x = p->left;
    } else {
        return t; // no conflict
    }

    if (u->color == red) { // push the conflict further up the tree
        p->color = black;
        u->color = black;
        t->color = red;
        return t;
    } else { // u->color == black
        if (p->left != x) { // If not the left child, then we need to make it the left child
            if (t->left == p) {
                t->left = rbtree_leftrot(t);
            } else {
                t->right = rbtree_leftrot(t);
            }
            // swap p and x in tree
            s = p->left;
            p->left = x->left;
            x->left = s;
            s = p->right;
            p->right = x->right;
            x->right = p;
            // Rotate about g,
            if (t->left == p) {
                rbtree_rightrot(t);
            } else { // t->right == p
                rbtree_leftrot(t);
            }
            // Switch the colors of t and p
            tmp = t->color;
            t->color = t->color;
            p->color = tmp;
            // return p as the new root
            return p;
        }
    }

}

struct rbtree *rbtree_insert(struct rbtree *t, int key, int value)
{
    struct rbtree *x = alloc_rbtree(red, NULL, NULL, key, value);

    t = insert_recur(t, x);
    if (t->color == red) {
        t->color = black;
    }
    return t;
}

struct rbtreee *delete(struct rbtree *t, int key)
{
    
}