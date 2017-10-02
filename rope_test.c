#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "rope.h"

// Functions to create test values

/** Return null
*/
struct rope *create_null_rope(void)
{
    return NULL;
}

/** Create an empty rope 
    @returns a rope with a single node consisting of the empty string ("")
*/
struct rope *create_empty_rope(void)
{
    return new_rope("");
}

/** Create a rope with a single character
    @return a rope consisting of a single character "a"
*/
struct rope *create_single_charactera(void)
{
    return new_rope("a");
}

/** Create a rope with a single character
    @return a rope consisting of a single character "b"
*/
struct rope *create_single_characterb(void)
{
    return new_rope("b");
}

/** Create a rope woth two leaf nodes, 'a' and 'b'
*/
struct rope *create_ab(void)
{
    struct rope_node *left = alloc_rope_node(1, NULL, NULL, "a");
    struct rope_node *right = alloc_rope_node(1, NULL, NULL, "b");
    struct rope *r = (struct rope *)malloc(sizeof(struct rope));
    r->head = alloc_rope_node(2, left, right, NULL);
    return r;
}

/** Create a rope with a single node, but multiple characters 
    Several variations of this function exist so that they
    can be used in tandem to create more complex scenarios for testing.
    @return a rope with multiple characters 
*/
struct rope *create_multichar_single_node1(void)
{
    return new_rope("foobar");
}

/** Create an invalid rope where the head has one null node
    Create a rope where the head has one null node
*/
struct rope *create_broken_rope_non_leaf(void)
{
    struct rope *r = (struct rope *)malloc(sizeof(struct rope));
    struct rope_node *left = alloc_rope_node(1, NULL, NULL, "a");
    r->head = alloc_rope_node(1, left, NULL, NULL);
    return r;
}

/** Create an invalid rope where a leaf has a single node
*/
struct rope *create_broken_rope_leaf(void)
{
    struct rope *r = (struct rope *)malloc(sizeof(struct rope));
    struct rope_node *valid = alloc_rope_node(1, NULL, NULL, "c");
    struct rope_node *left = alloc_rope_node(1, valid, NULL, "a");
    struct rope_node *right = alloc_rope_node(1, NULL, NULL, "b");
    r->head = alloc_rope_node(1, left, right, NULL);
    return r;
}

/** create an invalid rope where a leaf has the incorrect weight

*/
struct rope *create_broken_rope_leaf_weight(void)
{
    struct rope *r = (struct rope *)malloc(sizeof(struct rope));
    struct rope_node *right = alloc_rope_node(2, NULL, NULL, "a");
    struct rope_node *left = alloc_rope_node(1, NULL, NULL, "a");
    r->head = alloc_rope_node(3, left, right, NULL);
    return r;
}

/** create an invalid rope where a a non-leaf has the incorrect weight

*/
struct rope *create_broken_rope_non_leaf_weight(void)
{
    struct rope *r = (struct rope *)malloc(sizeof(struct rope));
    struct rope_node *right = alloc_rope_node(1, NULL, NULL, "a");
    struct rope_node *left = alloc_rope_node(1, NULL, NULL, "a");
    r->head = alloc_rope_node(3, left, right, NULL);
    return r;
}

/** Create a rope with two nodes, the one on the left only containing a single character
    @returns a rope with three nodes
*/
struct rope *create_single_char_left(void)
{
    struct rope_node *left = alloc_rope_node(1, NULL, NULL, "e");
    struct rope_node *right = alloc_rope_node(10, NULL, NULL, "fficiently");
    struct rope_node *head = alloc_rope_node(11, left, right, NULL);
    struct rope *r = (struct rope *)malloc(sizeof(struct rope));
    r->head = head;
    return r;
}

struct rope *create_single_char_right(void)
{
    struct rope_node *left = alloc_rope_node(10, NULL, NULL, "efficientl");
    struct rope_node *right = alloc_rope_node(1, NULL, NULL, "y");
    struct rope_node *head = alloc_rope_node(11, left, right, NULL);
    struct rope *r = (struct rope *)malloc(sizeof(struct rope));
    r->head = head;
    return r;
}

/** Create a rope of height 2, each of it's two children having three characters.
*/ 
struct rope *create_height2_chars3(void)
{
    struct rope_node *left = alloc_rope_node(3, NULL, NULL, "foo");
    struct rope_node *right = alloc_rope_node(3, NULL, NULL, "bar");
    struct rope_node *head = alloc_rope_node(6, left, right, NULL);
    struct rope *r = (struct rope *)malloc(sizeof(struct rope));
    r->head = head;
    return r;  
}

/** Create a rope of height 3

*/
struct rope *create_rope_height_3(void)
{
    return new_ropev(4, "abc", "def", "ghi", "jkl");
}

struct rope *substring_height_3_0_to_10(void)
{
    return new_ropev(4, "abc", "def", "ghi", "jk");
}

struct rope *substring_height_3_1_to_11(void)
{
    return new_ropev(4, "bc", "def", "ghi", "jkl");
}

struct rope *substring_height_2_0_to_4(void)
{
    return new_ropev(2, "foo", "ba");
}

struct rope *substring_height_1_to_5(void)
{
    return new_ropev(2, "oo", "bar");
}

struct rope *substring_height_2_to_3(void)
{
    return new_rope("o");
}

struct rope *substring_height_2_to_4(void)
{
    return new_ropev(2, "o", "b");
}

struct rope *substring_height_3_to_6(void)
{
    return new_rope("bar");
}

struct rope *substring_height_0_to_2(void)
{
    return new_rope("foo");
}

// Types to describe single tests
typedef struct rope *(*create_rope_func)(void);

struct is_rope_test {
    create_rope_func setup;
    bool expected;
};

struct rope_index_test {
    create_rope_func setup;
    int index;
    char expected;
};

struct rope_concat_test {
    create_rope_func arg1;
    create_rope_func arg2;
    create_rope_func expected;
};

struct rope_tostring_test {
    create_rope_func setup;
    char *expected;
};

struct rope_substring_test {
    create_rope_func setup;
    int lo;
    int hi;
    create_rope_func expected;
};

// tables of tests
#define NELEM(arr) (sizeof(arr)/sizeof(arr[0]))

struct is_rope_test is_rope_tests[] = {
    { // An empty rope
        .setup = create_null_rope,
        .expected = true
    },
    {
        .setup = create_empty_rope,
        .expected = true
    },
    {
        .setup = create_multichar_single_node1,
        .expected = true
    },
    {
        .setup = create_broken_rope_non_leaf,
        .expected = false
    },
    {
        .setup = *create_broken_rope_leaf,
        .expected = false
    },
    {
        .setup = create_broken_rope_leaf_weight,
        .expected = false
    },
    {
        .setup = create_broken_rope_non_leaf_weight,
        .expected = false
    }
};

struct rope_index_test rope_index_tests[] = {
    // error conditions
    { // A single empty node
        .setup = create_empty_rope,
        .index = 0,
        .expected = -1,
    },
    { // Out of bounds
        .setup = create_multichar_single_node1,
        .index = 55,
        .expected = -1
    },
    // depth of zero
    { // A rope with a single node and single character
        .setup = create_single_charactera,
        .index = 0,
        .expected = 'a'
    },
    {  // Rope with a single node, character in the middle of string
        .setup = create_multichar_single_node1,
        .index = 1,
        .expected = 'o'
    },
    {   // Rope with a single node, last character in string
        .setup = create_multichar_single_node1,
        .index = 5,
        .expected = 'r'
    },

    // depth of one
    {   // left node only has one char
        .setup = create_single_char_left,
        .index = 0,
        .expected = 'e'
    },
    {  // Right node only has one char
        .setup = create_single_char_right,
        .index = 10,
        .expected = 'y'
    },
    { // Middle of left
        .setup = create_height2_chars3,
        .index = 1,
        .expected = 'o'
    },
    { // middle of right
        .setup = create_height2_chars3,
        .index = 4,
        .expected = 'a'
    },
    { // first char
        .setup = create_height2_chars3,
        .index = 0,
        .expected = 'f'
    },
    { // last char
        .setup = create_height2_chars3,
        .index = 5,
        .expected = 'r'
    },
    // depth of two
    {
        .setup = create_rope_height_3,
        .index = 3,
        .expected = 'd'
    },
    {
        .setup = create_rope_height_3,
        .index = 2,
        .expected = 'c'
    },
    {
        .setup = create_rope_height_3,
        .index = 11,
        .expected = 'l'
    }
};

struct rope_concat_test rope_concat_tests[] = {
    {
        .arg1 = create_single_charactera,
        .arg2 = create_single_characterb,
        .expected = create_ab
    },
    {
        .arg1 = create_single_charactera,
        .arg2 = create_empty_rope,
        .expected = create_single_charactera
    }
};

struct rope_tostring_test rope_tostring_tests[] = {
    {
        .setup = create_empty_rope,
        .expected = ""
    },
    {
        .setup = create_single_charactera,
        .expected = "a"
    },
    {
        .setup = create_rope_height_3,
        .expected = "abcdefghijkl"
    },
    {
        .setup = create_height2_chars3,
        .expected = "foobar"
    },
    {
        .setup = create_single_char_right,
        .expected = "efficiently"
    },
    {
        .setup = create_single_char_left,
        .expected = "efficiently"
    }
};

struct rope_substring_test rope_substring_tests[] = {
    // empty result
    {
        .setup = create_empty_rope,
        .lo = 0,
        .hi = 0,
        .expected = create_empty_rope
    },
    {
        .setup = create_single_charactera,
        .lo = 0,
        .hi = 0,
        .expected = create_empty_rope
    },
    // same as entire string
    {
        .setup = create_single_charactera,
        .lo = 0,
        .hi = 1,
        .expected = create_single_charactera
    },
    {
        .setup = create_height2_chars3,
        .lo = 0,
        .hi = 6,
        .expected = create_height2_chars3
    },
    {
        .setup = create_rope_height_3,
        .lo = 0,
        .hi = 12,
        .expected = create_rope_height_3,
    },
    // entire string but first or last character
    {
        .setup = create_height2_chars3,
        .lo = 0,
        .hi = 5,
        .expected = substring_height_2_0_to_4
    },
    {
        .setup = create_height2_chars3,
        .lo = 1,
        .hi = 6,
        .expected = substring_height_1_to_5
    },
    {
        .setup = create_rope_height_3,
        .lo = 0,
        .hi = 11,
        .expected = substring_height_3_0_to_10
    },
    {
        .setup = create_rope_height_3,
        .lo = 1,
        .hi = 12,
        .expected = substring_height_3_1_to_11
    },
    // string is on the boundary between nodes (upper and lower)
    {
        .setup = create_height2_chars3,
        .lo = 2,
        .hi = 3,
        .expected = substring_height_2_to_3
    },
    {
        .setup = create_height2_chars3,
        .lo = 2,
        .hi = 4,
        .expected = substring_height_2_to_4
    },
    {
        .setup = create_height2_chars3,
        .lo = 3,
        .hi = 7,
        .expected = substring_height_3_to_6
    },
    {
        .setup = create_height2_chars3,
        .lo = 0,
        .hi = 3,
        .expected = substring_height_0_to_2
    }
};

int main_is_rope()
{
    struct rope *r;
    bool result;
    struct is_rope_test *test;
    int failed = 0;
    for (int i = 0; i < NELEM(is_rope_tests); ++i) {
        test = &is_rope_tests[i];
        r = test->setup();
        result = is_rope(r);
        if (result != test->expected) {
            printf("is_rope failed test %d: expected %d, got %d\n", i, test->expected, result);
            failed++;
        } else {
            printf("is_rope passed test %d\n", i);
        }
        free_rope(r, false);
    }
    return failed;
}

int main_rope_index()
{
    struct rope *r;
    int index;
    char result;
    struct rope_index_test *test;
    int failed = 0;
    for (int i = 0; i < NELEM(rope_index_tests); ++i) {
        test = &rope_index_tests[i];
        r = test->setup();
        index = test->index;
        result = rope_index(r, index);
        if (result != test->expected) {
            printf("rope_index failed test %d: expected %c, got %c\n", i, test->expected, result);
            failed++;
        } else {
            printf("rope_index passed test %d\n", i);
        }
        free_rope(r, false);
    }
    return failed;
}

int main_rope_concat()
{
    struct rope *r1, *r2, *res, *expect;
    struct rope_concat_test *test;
    int failed = 0;
    for (int i = 0; i < NELEM(rope_concat_tests); ++i) {
        test = &rope_concat_tests[i];
        r1 = test->arg1();
        r2 = test->arg2();
        expect = test->expected();
        res = rope_concat(r1, r2);
        if (!rope_equal(res, expect)) {
            printf("rope_concat failed test %d\n", i);
            failed++;
        } else {
            printf("rope_concat passed test %d\n", i);
        }
        free_rope(expect, false);
        free_rope(res, false);
        free(r1);
        free(r2);
    }
    return failed;
}

int main_rope_tostring()
{
    struct rope *r;
    char *result, *expected;
    struct rope_tostring_test *test;
    int failed = 0;
    for (int i = 0; i < NELEM(rope_tostring_tests); ++i) {
        test = &rope_tostring_tests[i];
        expected = test->expected;
        r = test->setup();
        result = rope_tostring(r);
        if (strcmp(result, expected)) {
            printf("rope_tostring failed test %d: expected %s, got %s\n", i, expected, result);
            failed++;
        } else {
            printf("rope_tostring passed test %d\n", i);
        }
    }
    return failed;
}

int main_rope_substring()
{
    struct rope *r, *expect, *result;
    int lo, hi;
    struct rope_substring_test *test;
    int failed = 0;
    for (int i = 0; i < NELEM(rope_substring_tests); ++i) {
        test = &rope_substring_tests[i];
        r = test->setup();
        lo = test->lo;
        hi = test->hi;
        result = rope_substring(r, lo, hi);
        expect = test->expected();
        if (!rope_equal(result, expect)) {
            printf("rope_substring failed test %d\n", i);
            failed++;
        } else {
            printf("rope_substring passed test %d\n", i);
        }
    }
    return failed;
}


/** Main method for running our test suite
    this will call main_*func name* for each function
*/
int main()
{
    int failed = 0;
    failed += main_is_rope();
    failed += main_rope_index();
    failed += main_rope_concat();
    failed += main_rope_tostring();
    failed += main_rope_substring();
    printf("%d tests failed\n", failed);
}