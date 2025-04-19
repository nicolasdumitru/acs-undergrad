// === Source Information ===
// Copyright (C) 2024 Nicolas Dumitru
// SPDX-License-Identifier: GPL-3.0-or-later

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
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

typedef struct BinTree BinTree;
struct BinTree {
    i32 key;
    BinTree *left;  // the left subtree
    BinTree *right; // the right subtree
};
// Note: In this particular implementation, the binary tree is a recursive
// structure. The `key` member is the key of the root node of the (sub)tree and
// `left` and `right` are pointers to the left and right subtrees respectively.
// Each subtree is also a BinTree. A NULL pointer indicates the absence of a
// subtree. The clarification regarding the recursive nature of the structure is
// particularly relevant for understanding design decisions made in the
// implementations of some functions below.

usize abs_diff(usize a, usize b) {
    return a >= b ? a - b : b - a;
}

typedef struct {
    bool answer; // TODO: maybe rename this?
    usize height;
} TreeStatus;

// This function uses a postorder tree walk to determine whether the tree is
// balanced. The left and right subtrees are visited recursively before it is
// possible to check whether the tree is balanced.
// Optimization: `height` in TreeStatus is greater than the actual height of the
// tree by 1 at all times. This eliminates the need for another conditional
// while preserving the correctness of the function.
TreeStatus BinTree_is_balanced_impl(const BinTree *tree) {
    const TreeStatus left = tree->left != NULL
                                ? BinTree_is_balanced_impl(tree->left)
                                : (TreeStatus){true, 0};
    // Optimization: There is no point in checking the right subtree if the left
    // is unbalanced anyway.
    const TreeStatus right = tree->right != NULL && left.answer
                                 ? BinTree_is_balanced_impl(tree->right)
                                 : (TreeStatus){left.answer, 0};

    return (TreeStatus){
        left.answer && right.answer && abs_diff(left.height, right.height) <= 1,
        1 + (left.height > right.height ? left.height : right.height)};
}

bool BinTree_is_balanced(BinTree *tree) {
    // Note: By convention, an empty tree is considered balanced.
    return tree != NULL ? BinTree_is_balanced_impl(tree).answer : true;
}

// This function uses a postorder tree walk to find the Lowest Common Ancestor
// (LCA) of nodes `node1` and `node2` in tree `tree`.
// Note: It is assumed that the input is valid and, thus both `node1` and
// `node2` are present in the tree.
BinTree *BinTree_lca(BinTree *tree, const BinTree *node1,
                     const BinTree *node2) {
    if (tree == NULL || tree == node1 || tree == node2) return tree;

    BinTree *left = BinTree_lca(tree->left, node1, node2);
    BinTree *right = BinTree_lca(tree->right, node1, node2);

    // If both left and right are not NULL, the root is the LCA.
    return left != NULL ? (right != NULL ? tree : left) : right;
}

int main(void) {
    return 0;
}
