// === Source Information ===
// Copyright (C) 2025 Nicolas Dumitru
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

// Ex. 1
usize abs_diff(usize a, usize b) {
    return a >= b ? a - b : b - a;
}

typedef struct {
    bool is_balanced;
    usize height;
} TreeBalanceStatus;

// This function uses a postorder tree walk to determine whether the tree is
// balanced. The left and right subtrees are visited recursively before it is
// possible to check whether the tree is balanced.
// Optimization: `height` in TreeStatus is greater than the actual height of the
// tree by 1 at all times. This eliminates the need for another conditional
// while preserving the correctness of the function.
TreeBalanceStatus BinTree_is_balanced_impl(const BinTree *tree) {
    const TreeBalanceStatus left = tree->left != NULL
                                       ? BinTree_is_balanced_impl(tree->left)
                                       : (TreeBalanceStatus){true, 0};
    // Optimization: There is no point in checking the right subtree if the left
    // is unbalanced anyway.
    const TreeBalanceStatus right =
        tree->right != NULL && left.is_balanced
            ? BinTree_is_balanced_impl(tree->right)
            : (TreeBalanceStatus){left.is_balanced, 0};

    return (TreeBalanceStatus){
        left.is_balanced && right.is_balanced &&
            abs_diff(left.height, right.height) <= 1,
        1 + (left.height > right.height ? left.height : right.height)};
}

bool BinTree_is_balanced(BinTree *tree) {
    // Note: By convention, an empty tree is considered balanced.
    return tree != NULL ? BinTree_is_balanced_impl(tree).is_balanced : true;
}

// Ex. 2
// Note [1]: By leveraging the inorder traversal order of nodes, this line
// ensures the BST property is checked for left subtrees after unwinding their
// cals and for right subtrees when winding their calls.
bool BinTree_is_bst_impl(BinTree *tree, BinTree **prev) {
    if (tree == NULL) return true;

    // The left subtree must be a BST.
    if (!BinTree_is_bst_impl(tree->left, prev)) return false;

    if (*prev != NULL && (*prev)->key > tree->key) return false;

    *prev = tree; // See Note [1] for clarification

    return BinTree_is_bst_impl(tree->right, prev);
}

// Ex. 3
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

// Ex. 5
typedef struct {
    usize depth;
    BinTree *parent;
} NodeStatus;

// This function uses preorder traversal to find the depth and parent of a node
// in the tree.
NodeStatus BinTree_find_node_impl(BinTree *tree, BinTree *node,
                                  BinTree *parent) {
    if (tree == node) return (NodeStatus){0, parent};

    NodeStatus result = tree->left != NULL
                            ? BinTree_find_node_impl(tree->left, node, tree)
                            : (NodeStatus){0, NULL};

    result = result.parent != NULL || tree->right == NULL
                 ? result
                 : BinTree_find_node_impl(tree->right, node, tree);

    result.depth += 1;

    return result;
}

bool BinTree_node_has_cousins_impl(BinTree *tree, BinTree *node,
                                   NodeStatus status, usize depth) {
    if (depth + 1 == status.depth) { // avoid unsigned underflow
        return tree != status.parent &&
               ((tree->left != NULL && tree->left != node) ||
                (tree->right != NULL && tree->right != node));
    }
    bool ans = false;
    if (depth + 1 < status.depth) {
        ans = tree->left != NULL ? BinTree_node_has_cousins_impl(
                                       tree->left, node, status, depth + 1)
                                 : ans;
        ans = ans || tree->right == NULL
                  ? ans
                  : BinTree_node_has_cousins_impl(tree->right, node, status,
                                                  depth + 1);
    }

    return ans;
}

bool BinTree_node_has_cousins(BinTree *tree, BinTree *node) {
    if (tree == NULL || node == NULL) return false;

    const NodeStatus status = BinTree_find_node_impl(tree, node, NULL);

    if (status.parent == NULL) return false;

    return BinTree_node_has_cousins_impl(tree, node, status, 0);
}

int main(void) {
    // Helper prev pointers for BST tests
    BinTree *prev = NULL;

    //
    // Test 1: Simple balanced BST
    //
    BinTree n1 = {.key = 1, .left = NULL, .right = NULL};
    BinTree n3 = {.key = 3, .left = NULL, .right = NULL};
    BinTree n2 = {.key = 2, .left = &n1, .right = &n3};
    BinTree *root1 = &n2;

    printf("Test 1: tree = {2,1,3}\n");
    printf("  is_balanced = %s\n",
           BinTree_is_balanced(root1) ? "true" : "false");
    prev = NULL;
    printf("  is_bst       = %s\n",
           BinTree_is_bst_impl(root1, &prev) ? "true" : "false");

    //
    // Test 2: Unbalanced tree
    //
    BinTree m4 = {.key = 4, .left = NULL, .right = NULL};
    BinTree m5 = {.key = 5, .left = &m4, .right = NULL};
    BinTree m6 = {.key = 6, .left = &m5, .right = NULL};
    BinTree *root2 = &m6;

    printf("\nTest 2: tree = {6,5,4} (skewed)\n");
    printf("  is_balanced = %s\n",
           BinTree_is_balanced(root2) ? "true" : "false");
    prev = NULL;
    printf("  is_bst       = %s\n",
           BinTree_is_bst_impl(root2, &prev) ? "true" : "false");

    //
    // Test 3: Not a BST
    //
    BinTree x1 = {.key = 1, .left = NULL, .right = NULL};
    BinTree x4 = {.key = 4, .left = NULL, .right = NULL};
    BinTree x3 = {.key = 3, .left = &x4, .right = NULL}; // 4 in left subtree
    BinTree x2 = {.key = 2, .left = &x1, .right = &x3};
    BinTree *root3 = &x2;

    printf("\nTest 3: tree = {2,1,3-with-4-as-left-child} (violates BST)\n");
    printf("  is_balanced = %s\n",
           BinTree_is_balanced(root3) ? "true" : "false");
    prev = NULL;
    printf("  is_bst       = %s\n",
           BinTree_is_bst_impl(root3, &prev) ? "true" : "false");

    //
    // Test 4: LCA in a larger tree
    //
    //      5
    //     / \
    //    2   8
    //   / \ / \
    //  1  3 6  9
    //
    BinTree t1 = {.key = 1, .left = NULL, .right = NULL};
    BinTree t3 = {.key = 3, .left = NULL, .right = NULL};
    BinTree t6 = {.key = 6, .left = NULL, .right = NULL};
    BinTree t9 = {.key = 9, .left = NULL, .right = NULL};
    BinTree t2 = {.key = 2, .left = &t1, .right = &t3};
    BinTree t8 = {.key = 8, .left = &t6, .right = &t9};
    BinTree t5 = {.key = 5, .left = &t2, .right = &t8};
    BinTree *root4 = &t5;

    // Pick two nodes to find LCA for:
    BinTree *a = &t1;
    BinTree *b = &t3;
    BinTree *lca = BinTree_lca(root4, a, b);
    printf("\nTest 4: LCA of %d and %d in tree {5,...}\n", a->key, b->key);
    if (lca) printf("  LCA = %d\n", lca->key);
    else printf("  LCA not found\n");

    printf("\n%d\n", BinTree_node_has_cousins(&t5, &t6));

    //
    // Test 5a: Small tree {2,1,3} – leaves should have NO cousins
    //
    printf("\nTest 5a: Cousins in small tree {2,1,3}\n");
    printf("  node %d has cousins = %s\n", n1.key,
           BinTree_node_has_cousins(root1, &n1) ? "true" : "false");
    printf("  node %d has cousins = %s\n", n3.key,
           BinTree_node_has_cousins(root1, &n3) ? "true" : "false");

    //
    // Test 5b: Large tree {5,2,8,1,3,6,9}
    //   Level 0: 5
    //   Level 1: 2, 8
    //   Level 2: 1,3, 6,9
    // – leaves under different parents at same depth (1 vs 6) ARE cousins.
    // – nodes at depth 1 (2 vs 8) are siblings, not cousins.
    //
    printf("\nTest 5b: Cousins in larger tree {5,2,8,1,3,6,9}\n");
    // those that DO have cousins:
    printf("  node %d has cousins = %s\n", t1.key,
           BinTree_node_has_cousins(root4, &t1) ? "true" : "false");
    printf("  node %d has cousins = %s\n", t6.key,
           BinTree_node_has_cousins(root4, &t6) ? "true" : "false");
    // those that do NOT:
    printf("  node %d has cousins = %s\n", t2.key,
           BinTree_node_has_cousins(root4, &t2) ? "true" : "false");
    printf("  node %d has cousins = %s\n", t5.key,
           BinTree_node_has_cousins(root4, &t5) ? "true" : "false");

    return 0;
}
