/*
 * Sorin Turda, grupa 30222
 * Arborele binar de cautare construit este balansat => inaltimea h = log(n). Functia build_tree face acest lucru, complexitate O(n)
 * functia os_select are complexitate O(h)
 * functia os_delete are complexitate O(h). dupa fiecare stergere, size-ul fiecarui nod se actualizeaza in timp logaritmic
 */
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "../Profiler.h"

#define MAX_SIZE 10000
#define STEP_SIZE 100
#define NR_TESTS 5

Profiler P("order statistics");

struct NodeT {
    int key;
    int size;
    NodeT *left;
    NodeT *right;
    NodeT *p;
};

NodeT *create(int key) {
    NodeT *p = new NodeT;
    p->left = nullptr;
    p->right = nullptr;
    p->p = nullptr;
    p->key = key;
    p->size = 0;
    return p;
}

void pp(NodeT *root, int indent) {
    if (root == nullptr)
        return;
    char s[indent + 1];
    memset(s, ' ', indent);
    s[indent] = '\0';
    printf("%s%d(%d)\n", s, root->key, root->size);
    pp(root->right, indent + 4);
    pp(root->left, indent + 4);
}

NodeT *find_min(NodeT *x, Operation ops) {
    ops.count(1);
    while (x->left) {
        x = x->left;
        ops.count(1);
    }
    return x;
}

NodeT *succesor(NodeT *&x, Operation ops) {
    ops.count(1);
    if (x->right)
        return find_min(x->right, ops);
    NodeT *y = x->p;
    ops.count(1);
    while (y && x == y->right) {
        ops.count(1);
        if (y == nullptr)
            break;
        ops.count(1);
        if (x != y->right)
            break;
        ops.count(2);
        x = y;
        y = y->p;
    }
    return y;
}

void update(NodeT *&x, Operation ops) {
    ops.count(1);
    if (x == nullptr)
        return;
    update(x->p, ops);
    ops.count(1);
    x->size--;
}

NodeT *build_tree(NodeT *&y, int left, int right, Operation ops) {
    if (left > right)
        return nullptr;
    int mid = left + (right - left) / 2;
    ops.count(4);
    NodeT *x = create(mid);
    x->left = build_tree(x, left, mid - 1, ops);
    x->right = build_tree(x, mid + 1, right, ops);
    x->p = y;
    x->size = right - left + 1;
    return x;
}

NodeT *os_select(NodeT *&x, int i, Operation ops) {
    int r = x->left ? x->left->size + 1 : 1;
    ops.count(2);
    if (i == r)
        return x;
    ops.count(1);
    if (i < r)
        return os_select(x->left, i, ops);
    return os_select(x->right, i - r, ops);
}

void os_delete(NodeT *&root, NodeT *&z, Operation ops) {
    NodeT *y, *x;
    if (!z->left || !z->right)
        y = z;
    else
        y = succesor(z, ops);
    ops.count(1);
    if (y->left)
        x = y->left;
    else
        x = y->right;
    ops.count(1);
    if (x) {
        x->p = y->p;
        ops.count(1);
    }
    if (!y->p) {
        ops.count(1);
        root = x;
    }
    else {
        if (y == y->p->left)
            y->p->left = x;
        else
            y->p->right = x;
        ops.count(2);
    }
    if (y != z) {
        z->key = y->key;
        ops.count(1);
    }
    ops.count(5);
    update(y, ops);
}

void perf() {
    for (int n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE) {
        Operation ops = P.createOperation("Operatii", n);
        for (int test = 0; test < NR_TESTS; test++) {
            NodeT *root = nullptr;
            root = build_tree(root, 1, n, ops);
            for (int i = 1; i <= n; i++) {
                NodeT *x = os_select(root, rand() % (n - i + 1) + 1, ops);
                os_delete(root, x, ops);
            }
        }
    }
    P.divideValues("Operatii", NR_TESTS);
    P.showReport();
}

void demo() {
    NodeT *root = nullptr;
    int n = 11;
    Operation ops = P.createOperation("", n);
    pp(root, 0);
    root = build_tree(root, 1, n, ops);
    pp(root, 0);
    for (int i = 0; i < 3; i++) {
        int x = rand() % n + 1;
        NodeT *y;
        printf("x = %d\n", x);
        y = os_select(root, x, ops);
        os_delete(root, y, ops);
        pp(root, 0);
        n--;
    }
}

int main() {
    srand(time(nullptr));
    // demo();
    perf();
    return 0;
}