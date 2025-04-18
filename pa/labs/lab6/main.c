// === Source Information ===
// Copyright (C) 2024 Nicolas Dumitru
// SPDX-License-Identifier: GPL-3.0-or-later

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

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

typedef struct Vertex Vertex;
struct Vertex {
    Vertex *left;
    Vertex *right;
};

typedef struct {
    bool ans;
    usize height;
} AnsHeightPair;

usize abs_diff(usize a, usize b) {
    if (a < b) {
        usize aux = a;
        a = b;
        b = aux;
    }
    return a - b;
}

AnsHeightPair is_balanced(Vertex *root) {
    AnsHeightPair result = (AnsHeightPair){true, 0};
    if (root->left != NULL && root->right != NULL) {
        AnsHeightPair l, r;
        l = is_balanced(root->left);
        r = is_balanced(root->right);
        if (!l.ans || !r.ans || abs_diff(l.height, r.height) > 1)
            return (AnsHeightPair){false, 0};
        result = (l.height > r.height) ? l : r;
    } else if (root->left != NULL) {
        result = is_balanced(root->left);
    } else if (root->right != NULL) {
        result = is_balanced(root->right);
    }

    result.height += 1;

    return result;
}

#define N 5
int main(void) {

    Vertex v[N + 1] = {{NULL, NULL}, {NULL, NULL}, {NULL, NULL}, {NULL, NULL}, {NULL, NULL}, {NULL, NULL}};
    for (usize i = 1; i <= N / 2; i += 1) {
        v[i].left = &v[2 * i];
        v[i].right = &v[2 * i + 1];
    }

    AnsHeightPair r = is_balanced(&v[1]);
    printf("%d\n", r.ans);

    return 0;
}
