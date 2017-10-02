#ifndef ROPE_H
#define ROPE_H

struct rope_node {
    struct rope_node *left;
    struct rope_node *right;
    int weight;
    char *data;
};

struct rope {
    struct rope_node *head;
};

struct rope *new_rope(char *s);
struct rope_node *alloc_rope_node(int weight, struct rope_node *left, struct rope_node *right, char *data);
struct rope *new_ropev(int argc, ...);
void free_rope(struct rope *r, bool free_strings);

bool is_rope(struct rope *r);
char rope_index(struct rope *r, int index);
struct rope *rope_concat(struct rope *r1, struct rope *r2);
bool rope_equal(struct rope *r1, struct rope *r2);
char *rope_tostring(struct rope *r);
struct rope *rope_substring(struct rope *r, int lo, int hi);

#endif /* ROPE_H */