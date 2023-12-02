#include <iostream>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "../Profiler.h"

#define MAX_SIZE 10000
#define STEP_SIZE 100
#define NR_TESTS 5

Profiler P("tema");

struct NodeT {
    int key;
    int size;
    NodeT *left;
    NodeT *right;
    NodeT *parent;
};

NodeT *create(int key) {
    NodeT *p = new NodeT;
    p->left = nullptr;
    p->right = nullptr;
    p->parent = nullptr;
    p->key = key;
    return p;
}

void insert(NodeT *&x, int key, NodeT *parent) {
    if (x == nullptr) {
        x = create(key);
        x->parent = parent;
        return;
    }
    if (x->key < key)
        insert(x->right, key, x);
    else
        insert(x->left, key, x);
}

void inorder_recursiv(NodeT *x) {
    if (!x)
        return;
    inorder_recursiv(x->left);
    printf("%d ", x->key);
    inorder_recursiv(x->right);
}

void inorder_iterativ(NodeT *x) {
    if (!x)
        return;
    NodeT *temp, *curr = x, *pre;

    while (curr != nullptr) {
        if (curr->left == nullptr) {
            printf("%d ", curr->key);
            curr = curr->right;
        }

        else {
            pre = curr->left;
            while (pre->right != nullptr)
                pre = pre->right;
            pre->right = curr;
            temp = curr;
            curr = curr->left;
            temp->left = nullptr;
        }
    }
}

void demo() {
    NodeT *root = nullptr;
    int n = 10;
    int v[n];
    FillRandomArray(v, n, 10, 50000, true);
    for (int i = 0; i < n; i++)
        insert(root, v[i], root);
    // pp(root, 0);
    inorder_recursiv(root);
    puts("");
    inorder_iterativ(root);
}

int main() {
    srand(time(nullptr));
    demo();
    return 0;
}