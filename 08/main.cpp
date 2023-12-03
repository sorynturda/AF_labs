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

void insertion_sort(int a[], int n) {
    for (int i = 1; i < n; i++) {
        int x = a[i];
        // opAttr.count();
        int j = i - 1;
        while (j >= 0 && a[j] > x) {
            a[j + 1] = a[j];
            j--;
        }
        // opAttr.count();
        a[j + 1] = x;
    }
}

int partition(int a[], int left, int right) {
    int x = a[right]; // pivot elementul cel mai din dreapta
    // atrQs.count();
    int i = left - 1;
    for (int j = left; j <= right - 1; j++) {
        // compQs.count();
        if (a[j] <= x) {
            i++;
            // atrQs.count(3);
            std::swap(a[i], a[j]);
        }
    }
    // atrQs.count();
    std::swap(a[i + 1], a[right]); // punem pivotul la locul lui
    return i + 1; // pozitia unde e amplasat pivotul
}


void quicksort(int a[], int left, int right) {
    if (left >= right)
        return;
    if (right - left + 1 <= 30) {
        insertion_sort(a + left, right - left + 1);
        return;
    }
    int q = partition(a, left, right);
    quicksort(a, left, q - 1);
    quicksort(a, q + 1, right);
}

void perf() {
    int n = 10;
    int a[n];
    FillRandomArray(a, n);
    quicksort(a, 0, n - 1);
    for (int i = 0; i < n; i++)
        printf("%d ", a[i]);
    puts("");
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
    // demo();
    perf();
    return 0;
}