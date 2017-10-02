#ifndef RBTREE_H
#define RBTREE_H

enum color {
    red,
    black
};

struct rbtree {
    enum color color;
    struct rbtree *left;
    struct rbtree *right;
    int key;
    int value;
};

struct rbtree *alloc_rbtree(enum color c, struct rbtree *left, struct rbtree *right, int key, int value);
void free_rbtree(struct rbtree *t);
int rbtree_lookup(struct rbtree *t, int key);
struct rbtree *rbtree_insert(struct rbtree *t, int key, int value);
bool rbtree_equal(struct rbtree *t1, struct rbtree *t2);

bool rbtree_valid_coloring(struct rbtree *t);
struct rbtree *rbtree_rightrot(struct rbtree *t);
struct rbtree *rbtree_leftrot(struct rbtree *t);

#endif /* RBTREE_H */