#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>

#include "rope.h"

#define is_leaf_node(n) (!(n)->left && !(n)->right)

void free_rope_node(struct rope_node *n, bool free_strings)
{
    if (!n)
        return;
    free_rope_node(n->left, free_strings);
    free_rope_node(n->right, free_strings);
    if (n->data && free_strings)
        free(n->data);
    free(n);
}

void free_rope(struct rope *r, bool free_strings)
{
    if (!r)
        return;
    free_rope_node(r->head, free_strings);
    free(r);
}

struct rope_node *new_rope_node(char *s)
{
  struct rope_node *n = (struct rope_node *)malloc(sizeof(struct rope_node));
  n->data = s;
  n->left = NULL;
  n->right = NULL;
  n->weight = strlen(s);
  return n;
}

/** Create a new rope containing the given string
    Assumes the new rope's memory doesn't need to be freed
    @return A new rope, whose memory doesn't need to be freed.
*/ 
struct rope *new_rope(char *s)
{
    struct rope *r =  (struct rope *)malloc(sizeof(struct rope));
    if (!r)
        return NULL;
    r->head = strcmp(s, "") ? new_rope_node(s) : NULL;
    return r;
}

static int node_weight(struct rope_node *n)
{
  return n ? n->weight : 0;
}

struct rope_node *new_rope_nodev(int argc, char **args)
{
    struct rope_node *r = (struct rope_node *)malloc(sizeof(struct rope_node));
    if (argc == 1) {
        r->left = NULL;
        r->right = NULL;
        r->weight = strlen(*args);
        r->data = *args;
    } else {
        int args_remaining = argc/2;
        r->left = new_rope_nodev(args_remaining, args);
        r->right = new_rope_nodev(args_remaining, args+args_remaining);
        r->weight = r->left->weight + r->right->weight;
        r->data = NULL;
    }
    return r;
}

/** Create a new rope, with a variable number of arguments
    @param argc The number of arguments this function is passing.
    
*/
struct rope *new_ropev(int argc, ...)
{
    va_list ap;
    int i = log(argc)/log(2);
    if (i % 2 != 0 && i != 1) {
        return NULL;
    }
    i = argc;
    va_start(ap, argc);
    char **args = (char **)malloc(argc*sizeof(char *));
    char **p = args;
    while (i--) {
        *p++ = va_arg(ap, char *);
    }
    p = args;
    struct rope *r = (struct rope *)malloc(sizeof(struct rope));
    r->head = new_rope_nodev(argc, p);

    va_end(ap);
    free(args);
    return r;
}

static bool is_rope_node(struct rope_node *n)
{
    if (!n)
        return true;
    if (n->left || n->right) {  // concatenation node
        if (n->data || !n->left || !n->right)            // can't have any data
            return false;
        int weight = node_weight(n->left) + node_weight(n->right);
        if (weight != n->weight) // weight must equal the same as both subtrees
            return false;
        return is_rope_node(n->left) && is_rope_node(n->right);  // left and right subtrees must be ropes
    } else if (!n->left && !n->right) {

    }
    // leaf node
    return n->data && n->weight == strlen(n->data) ? true : false;
}

bool is_rope(struct rope *r)
{
    return !r || is_rope_node(r->head);
}

char rope_index(struct rope *r, int index)
{
    if (!r || !r->head)
        return -1;
    struct rope_node *n = r->head;
    while (n) {
        if (n->left && n->right) {          // concatenation node
            if (index < n->left->weight) {  // in left subtree
                n = n->left;
            } else {
                index -= n->left->weight;   // if in right subtree, subtract weight of left subtree
                n = n->right;
            }
        } else { // leaf node
            return index < 0 || index >= n->weight ? -1 : n->data[index];
        }
    }
    return -1;  // no such node
}

int rope_length(struct rope *r)
{
    return r && r->head ? r->head->weight : 0;
}

bool rope_node_equal(struct rope_node *n1, struct rope_node *n2)
{
    int weight;
    char *s, *p;
    if (!n1 && !n2)
        return true;
    if (is_leaf_node(n1) || is_leaf_node(n2)) {
        if (is_leaf_node(n1) != is_leaf_node(n2))
            return false;
        if (n1->weight != n2->weight)
            return false;
        weight = n1->weight;
        p = n1->data, s = n2->data;
        while (weight--)
            if (*p++ != *s++)
                return false;
        return true;
    }
    if (n1->weight != n2->weight)
        return false;
    return rope_node_equal(n1->left, n2->left) && rope_node_equal(n1->right, n2->right);
}

bool rope_equal(struct rope *r1, struct rope *r2)
{
    return rope_node_equal(r1->head, r2->head);
}

struct rope_node *copy_rope_node(struct rope_node *n)
{
    if (!n)
        return NULL;
    struct rope_node *ret = (struct rope_node *)malloc(sizeof(struct rope_node));
    ret->left = copy_rope_node(n->left);
    ret->right = copy_rope_node(n->right);
    ret->data = n->data;
    ret->weight = ret->data ? strlen(ret->data) : ret->left->weight + ret->right->weight;
    return ret;
}

struct rope *rope_copy(struct rope *r)
{
    struct rope *ret = (struct rope *)malloc(sizeof(struct rope));
    ret->head = copy_rope_node(r->head);
    if (r->head && !ret->head) {
        free(ret);
        return NULL;
    }
    return ret;
}

struct rope *rope_concat(struct rope *r1, struct rope *r2)
{
    if (!r1 || !r2)
        return NULL;
    if (!r1->head)
        return rope_copy(r2);
    if (!r2->head)
        return rope_copy(r1);
    struct rope *r;
    if ((r = (struct rope *)malloc(sizeof(struct rope))) == NULL)
        return NULL;
    if ((r->head = alloc_rope_node(r1->head->weight+r2->head->weight, r1->head, r2->head, NULL)) == NULL) {
        free(r);
        return NULL;
    }
    return r;
}

void rope_tostring_traverse(struct rope_node *n, char **c)
{
  if (!n)
    return;
  rope_tostring_traverse(n->left, c);
  char *copy = n->data;
  if (n->data) {
    while (copy - n->data < n->weight) {
        **c = *copy;
        *c += 1;
        copy++;
    }
  }
  rope_tostring_traverse(n->right, c);
}

char *rope_tostring(struct rope *r)
{
    char *ret;
    if (!r || !r->head) {
        ret = (char *)malloc(1);
        strcpy(ret, "");
        return ret;
    }
    ret = (char *)malloc(sizeof(char)*r->head->weight+1);
    char *p = ret;
    rope_tostring_traverse(r->head, &p);
    *p = '\0';
    return ret;
}

struct rope_node *alloc_rope_node(int weight, struct rope_node *left, struct rope_node *right, char *data)
{
    struct rope_node *ret;
    if ((ret = (struct rope_node *)malloc(sizeof(struct rope_node))) == NULL)
        return NULL;  
    ret->weight = weight;
    ret->left = left;
    ret->right = right;
    ret->data = data;
    return ret;
}

struct rope_node *substring_recur(struct rope_node *n, int lo, int hi)
{
    if (!n)
        return NULL;
    struct rope_node *left, *right, *ret;
    if (is_leaf_node(n)) {
        if (lo <= 0 && hi == 0)
            return NULL;
        ret = n;
        if (lo != -1 && lo > 0) { // need to realloc node
            ret = alloc_rope_node(n->weight-lo, NULL, NULL, n->data+lo);
        }
        if (hi != -1 && hi > 0 && hi < ret->weight) {
            if (ret == n) {
                ret = alloc_rope_node(hi, NULL, NULL, n->data);
            } else { // node contains lo as well
                ret->weight -= ret->weight - hi;
            }
        }
        return ret;
    }
    ret = NULL;
    int recur_lo, recur_hi;
    // Figure out if we need to recurse
    if ((lo >= 0 && lo < n->left->weight) || hi < n->left->weight) { // Is hi or lo contained in the left subtree?
        // If we recurse on the left subtree, it must contain lo. So we must check if it contains hi as well
        recur_hi = hi < 0 || hi >= n->left->weight ? -1 : hi - n->right->weight;
        left = substring_recur(n->left, lo, recur_hi);
    } else if (lo >= n->left->weight && hi >= n->left->weight)
        left = NULL; 
    else {
        left = n->left;
    }
    if (lo >= n->left->weight || hi >= n->left->weight) {
        // Similar to above, if we're recursing on the right subtee it must contain hi. Check if it contains lo
        recur_lo = lo < 0 || lo < n->right->weight ? -1 : lo - n->left->weight;
        right = substring_recur(n->right, recur_lo, hi-n->left->weight);
    } else {
        right = n->right;
    }
    // Look the results from this node's descendants. If a child was re-allocated, then
    // this node needs to be too.
    if (!right)
        return left;
    if (!left)
        return right;
    if (left != n->left) {
        if ((ret = (struct rope_node *)malloc(sizeof(struct rope_node))) == NULL)
            return NULL;
        ret->left = left;
        ret->right = n->right;
    }
    if (right != n->right) {
        if (!ret) {
            if ((ret = (struct rope_node *)malloc(sizeof(struct rope_node))) == NULL)
                return NULL;
            ret->left = left;
        }
        ret->right = right;
    }
    if (!ret) {
        ret = n;
    } else {
        ret->weight = ret->right->weight + ret->left->weight;
    }

    return ret;
}

struct rope *rope_substring(struct rope *r, int lo, int hi)
{
    struct rope *ret;
    if (!r || !r->head) {
        return lo == 0 && hi == 0 ? r : NULL;
    }
    if ((ret = (struct rope *)malloc(sizeof(struct rope))) == NULL)
        return NULL;
    
    ret->head = substring_recur(r->head, lo, hi);
    return ret;
}
