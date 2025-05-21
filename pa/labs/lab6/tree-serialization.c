// === Source Information ===
// Copyright (C) 2025 Nicolas Dumitru
// SPDX-License-Identifier: GPL-3.0-or-later

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef size_t usize;
typedef ptrdiff_t isize;
typedef uint8_t u8; // std input streams treat this as char
typedef int8_t i8;  // std input streams treat this as char
typedef uint16_t u16;
typedef int16_t i16;
typedef uint32_t u32;
typedef int32_t i32;
typedef uint64_t u64;
typedef int64_t i64;

typedef struct BinTree {
    i32 key;
    struct BinTree *left;
    struct BinTree *right;
} BinTree;

BinTree *BinTree_new(void) { // pseudo-constructor
    BinTree *tree = (BinTree *)malloc(sizeof(BinTree));
    if (tree == NULL) exit(1);

    tree->key = 0;
    tree->left = NULL;
    tree->right = NULL;

    return tree;
}

void BinTree_destroy(BinTree *tree) { // pseudo-destructor
    if (tree == NULL) return;

    BinTree_destroy(tree->left);
    BinTree_destroy(tree->right);

    free(tree);
}

typedef struct {
    usize length;
    usize capacity;
    char *data;
} String;

String String_new(void) { // pseudo-constructor
    char *s = (char *)malloc(sizeof(char));
    if (s == NULL) exit(1);

    s[0] = '\0';

    return (String){.length = 0, .capacity = 1, .data = s};
}

void String_destroy(String *s) {
    s->length = 0;
    s->capacity = 0;
    free(s->data);
}

void String_append(String *s, char buf[]) {
    usize length = strlen(buf);
    if (s->length + length > s->capacity) {
        s->capacity *= 2;
        s->data = (char *)realloc(s->data, s->capacity);
        if (s->data == NULL) exit(1);
    }

    strcat(s->data + s->length, buf);
    s->length += length;
}

#define capacity ((usize)12)
const char NULLTREE = '#';
const char SEPARATOR = ',';

void BinTree_serialize_impl(BinTree *tree, String *s) {
    char buf[capacity];
    if (tree == NULL) {
        snprintf(buf, capacity, "%c%c", NULLTREE, SEPARATOR);
        String_append(s, buf);
        return;
    }
    snprintf(buf, capacity, "%d%c", tree->key, SEPARATOR);
    String_append(s, buf);

    BinTree_serialize_impl(tree->left, s);
    BinTree_serialize_impl(tree->right, s);
}

String BinTree_serialize(BinTree *tree) {
    String s = String_new();
    BinTree_serialize_impl(tree, &s);

    return s;
}

BinTree *BinTree_deserialize_impl(String *s, usize *pos) {
    if (s->data[*pos] == NULLTREE) {
        *pos += 2;
        return NULL;
    }

    BinTree *tree = BinTree_new();
    for (; s->data[*pos] != SEPARATOR; *pos += 1) {
        tree->key = 10 * tree->key + (s->data[*pos] - '0');
    }
    *pos += 1; // go past the separator

    tree->left = BinTree_deserialize_impl(s, pos);
    tree->right = BinTree_deserialize_impl(s, pos);

    return tree;
}

BinTree *BinTree_deserialize(String *s) {
    usize pos = 0;

    return BinTree_deserialize_impl(s, &pos);
}

// Helper to build a small tree for testing:
//       1
//      / \
//     2   3
//        /
//       4
BinTree *make_test_tree(void) {
    BinTree *n1 = BinTree_new();
    n1->key = 1;
    BinTree *n2 = BinTree_new();
    n2->key = 2;
    BinTree *n3 = BinTree_new();
    n3->key = 3;
    BinTree *n4 = BinTree_new();
    n4->key = 4;
    n1->left = n2;
    n1->right = n3;
    n3->left = n4;

    return n1;
}

// Simple preorder print to verify structure
void BinTree_print_preorder(BinTree *t) {
    if (t == NULL) {
        printf("# ");
        return;
    }
    printf("%d ", t->key);

    BinTree_print_preorder(t->left);
    BinTree_print_preorder(t->right);
}

int main(void) {
    // 1) Build tree, serialize it
    BinTree *orig = make_test_tree();
    String s = BinTree_serialize(orig);
    printf("Serialized: %s\n", s.data);

    // 2) Deserialize back
    BinTree *copy = BinTree_deserialize(&s);

    // 3) Serialize the copy again to prove it matches
    String s2 = BinTree_serialize(copy);
    printf("Re-serialized: %s\n", s2.data);

    // 4) Print preorders
    printf("Original preorder: ");
    BinTree_print_preorder(orig);
    printf("\nCopy preorder:     ");
    BinTree_print_preorder(copy);
    printf("\n");

    BinTree_destroy(orig);
    BinTree_destroy(copy);

    String_destroy(&s);
    String_destroy(&s2);

    return 0;
}
