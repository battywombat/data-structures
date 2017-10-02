#include <stdio.h>
#include <stdbool.h>

#define NELEM(arr) (sizeof(arr)/sizeof(arr[0]))

#include "rbtree.h"

typedef struct rbtree *(*rbtree_creator)(void);

// create_rbtree functions
struct rbtree *create_null_tree(void)
{
    return NULL;
}

struct rbtree *create_single_node(void)
{
    return alloc_rbtree(black, NULL, NULL, 1, 2);
}

struct rbtree *create_invalid_single_node(void)
{
    return alloc_rbtree(red, NULL, NULL, 3, 4);
}

struct rbtree *create_invalid_height2(void)
{
    struct rbtree *left = alloc_rbtree(black, NULL, NULL, 5, 6);
    struct rbtree *right = alloc_rbtree(black, NULL, NULL, 7, 8);
    return alloc_rbtree(red, left, right, 9, 10);
}

struct rbtree *create_valid_height2(void)
{
    struct rbtree *left = alloc_rbtree(red, NULL, NULL, 5, 6);
    struct rbtree *right = alloc_rbtree(red, NULL, NULL, 7, 8);
    return alloc_rbtree(black, left, right, 9, 10);
}

struct rbtree *create_balanced_height3(void)
{
    struct rbtree *left = create_invalid_height2();
    struct rbtree *right = create_invalid_height2();
    return alloc_rbtree(black, left, right, 20, 55);
}

struct rbtree *create_unbalanced_height3(void)
{
    struct rbtree *left = create_invalid_height2();
    struct rbtree *right = alloc_rbtree(red, NULL, NULL, 55, 33);
    return alloc_rbtree(black, left, right, 1, 2);   
}

// rbtree_valid_coloring

struct rbtree_valid_coloring_test {
    rbtree_creator input;
    bool expected;
} valid_coloring_tests[] = {
    {
        .input = create_null_tree,
        .expected = true
    },
    {
        .input = create_single_node,
        .expected = true
    },
    {
        .input = create_invalid_single_node,
        .expected = false
    },
    {
        .input = create_invalid_height2,
        .expected = false
    },
    {
        .input = create_valid_height2,
        .expected = true
    },
    {
        .input = create_valid_height2,
        .expected = true
    },
    {
        .input = create_balanced_height3,
        .expected = true
    },
    {
        .input = create_unbalanced_height3,
        .expected = false
    }
};

int main_rbtree_valid_coloring()
{
    struct rbtree *t;
    bool result;
    int failed = 0;
    struct rbtree_valid_coloring_test *test;
    for (int i = 0; i < NELEM(valid_coloring_tests); ++i) {
        test = &valid_coloring_tests[i];
        t = test->input();
        result = rbtree_valid_coloring(t);
        if (result != test->expected) {
            printf("rbtree_valid_coloring failed test %d: expected %d, but got %d\n", i, test->expected, result);
            failed++;
        } else
            printf("rbtree_valid_coloring passed test %d\n", i);
    }
    return failed;
}

// rbtree_rightrot

struct rbtree *rightrot_result(void)
{
}

struct rbtree_rightrot_test {
    rbtree_creator input;
    rbtree_creator expected;
} rbtree_rightrot_tests[] = {
    {
        .input = 
    },
    {}
};

int main_rbtree_rightrot()
{
    struct rbtree *input, *expected;
    struct rbtree_rightrot_test *test = rbtree_rightrot_tests;
    int failed = 0;
    while (test++) {
        input = test->input();
        expected = test->expected();
        input = rbtree_rightrot(input);
        if (!rbtree_equal(input, expected)) {
            printf("rbtree_rightrot failed test %ld\n", test - rbtree_rightrot_tests);
            failed++;
        } else {
            printf("rbtree_rightrot passed test %ld\n", test - rbtree_rightrot_tests);
        }
    }
    return failed;
}


// main

int main()
{
    int failed = 0;
    failed += main_rbtree_valid_coloring();
    printf("%d tests failed\n", failed);
}