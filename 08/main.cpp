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

void copy(int a[NR_TESTS][MAX_SIZE], int b[NR_TESTS][MAX_SIZE], int n) {
    for (int i = 0; i < NR_TESTS; i++)
        for (int j = 0; j < n; j++)
            a[i][j] = b[i][j];
}

void copy(int *a, int *b, int n) {
    for (int i = 0; i < n; i++)
        a[i] = b[i];
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

void inorder_recursiv(NodeT *x, Operation ops) {
    if (!x)
        return;
    inorder_recursiv(x->left, ops);
    // printf("%d ", x->key);
    ops.count();
    inorder_recursiv(x->right, ops);
}

void inorder_iterativ(NodeT *x, Operation ops) {
    if (!x)
        return;
    NodeT *temp, *curr = x, *pre;

    while (curr != nullptr) {
        if (curr->left == nullptr) {
            // printf("%d ", curr->key);
            ops.count();
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

void insertion_sort(int a[], int n, Operation ops) {
    for (int i = 1; i < n; i++) {
        int x = a[i];
        ops.count();
        int j = i - 1;
        while (j >= 0) {
            ops.count();
            if (a[j] > x) {
                ops.count();
                a[j + 1] = a[j];
                j--;
            }
            else break;
        }
        ops.count();
        a[j + 1] = x;
    }
}

void insertion_sort(int a[], int n) {
    for (int i = 1; i < n; i++) {
        int x = a[i];
        int j = i - 1;
        while (j >= 0 && a[j] > x) {
            a[j + 1] = a[j];
            j--;
        }
        a[j + 1] = x;
    }
}

int partition(int a[], int left, int right, Operation ops) {
    int x = a[right]; // pivot elementul cel mai din dreapta
    ops.count();
    int i = left - 1;
    for (int j = left; j <= right - 1; j++) {
        ops.count();
        if (a[j] <= x) {
            i++;
            ops.count(3);
            std::swap(a[i], a[j]);
        }
    }
    ops.count();
    std::swap(a[i + 1], a[right]); // punem pivotul la locul lui
    return i + 1; // pozitia unde e amplasat pivotul
}

int partition(int a[], int left, int right) {
    int x = a[right]; // pivot elementul cel mai din dreapta
    int i = left - 1;
    for (int j = left; j <= right - 1; j++) {
        if (a[j] <= x) {
            i++;
            std::swap(a[i], a[j]);
        }
    }
    std::swap(a[i + 1], a[right]); // punem pivotul la locul lui
    return i + 1; // pozitia unde e amplasat pivotul
}

void quicksort(int a[], int left, int right, Operation ops) {
    if (left >= right)
        return;
    int q = partition(a, left, right, ops);
    quicksort(a, left, q - 1, ops);
    quicksort(a, q + 1, right, ops);
}

void quicksort(int a[], int left, int right) {
    if (left >= right)
        return;
    int q = partition(a, left, right);
    quicksort(a, left, q - 1);
    quicksort(a, q + 1, right);
}

void quicksort_h(int a[], int left, int right, Operation ops) {
    if (left >= right)
        return;
    if (right - left + 1 <= 30) {
        insertion_sort(a + left, right - left + 1, ops);
        return;
    }
    int q = partition(a, left, right, ops);
    quicksort_h(a, left, q - 1, ops);
    quicksort_h(a, q + 1, right, ops);
}

void quicksort_h(int a[], int left, int right) {
    if (left >= right)
        return;
    if (right - left + 1 <= 30) {
        insertion_sort(a + left, right - left + 1);
        return;
    }
    int q = partition(a, left, right);
    quicksort_h(a, left, q - 1);
    quicksort_h(a, q + 1, right);
}

void quicksort_h(int a[], int left, int right, int size) {
    if (left >= right)
        return;
    if (right - left + 1 <= size) {
        insertion_sort(a + left, right - left + 1);
        return;
    }
    int q = partition(a, left, right);
    quicksort_h(a, left, q - 1, size);
    quicksort_h(a, q + 1, right, size);
}

void perf_1() {
    int v[MAX_SIZE];
    for (int n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE)
        for (int test = 0; test < NR_TESTS; test++) {
            FillRandomArray(v, n);
            NodeT *root = nullptr;
            for (int i = 0; i < n; i++)
                insert(root, v[i], root);
            Operation ops1 = P.createOperation("recursiv", n);
            inorder_recursiv(root, ops1);
            Operation ops2 = P.createOperation("iterativ", n);
            inorder_iterativ(root, ops2);
        }
    P.divideValues("recursiv", NR_TESTS);
    P.divideValues("iterativ", NR_TESTS);
    P.createGroup("parcurgeri-arbore", "recursiv", "iterativ");
}

void perf_2() {
    // int v[MAX_SIZE];
    int v[NR_TESTS][MAX_SIZE], a[NR_TESTS][MAX_SIZE];
    for (int n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE) {
        for (int test = 0; test < NR_TESTS; test++)
            FillRandomArray(v[test], n);
        copy(a, v , n);
        for (int test = 0; test < NR_TESTS; test++) {
            Operation ops = P.createOperation("operatii-quicksort_h", n);
            quicksort_h(a[test], 0, n - 1, ops);
        }
        copy(a, v , n);
        for (int test = 0; test < NR_TESTS; test++) {
            Operation ops = P.createOperation("operatii-quicksort", n);
            quicksort(a[test], 0, n - 1, ops);
        }
        copy(a, v , n);
        P.startTimer("timp-quicksort_h", n);
        for (int test = 0; test < 200; test++) {
            copy(a[0], v[0], n);
            quicksort_h(a[0], 0, n - 1);
        }
        P.stopTimer("timp-quicksort_h", n);
        copy(a, v , n);
        P.startTimer("timp-quicksort", n);
        for (int test = 0; test < 200; test++) {
            copy(a[0], v[0], n);
            quicksort(a[0], 0, n - 1);
        }
        P.stopTimer("timp-quicksort", n);
    }
    P.divideValues("operatii-quicksort_h", NR_TESTS);
    P.divideValues("operatii-quicksort", NR_TESTS);
    P.createGroup("operatii", "operatii-quicksort", "operatii-quicksort_h");
    P.createGroup("timp", "timp-quicksort", "timp-quicksort_h");
}

void perf_3() {
    const int N = 1000;
    // int v[N][MAX_SIZE], a[N][MAX_SIZE];
    int a[MAX_SIZE];
    // for (int i = 0; i < N; i++)
    //     FillRandomArray(v[i], MAX_SIZE);
    for (int i = 0; i <= 1000; i++) {
        // copy(a, v, MAX_SIZE);
        P.startTimer("functie", i);
        for (int test = 0; test < N; test++) {
            FillRandomArray(a, MAX_SIZE);
            quicksort_h(a, 0, MAX_SIZE - 1, i);
        }
        P.stopTimer("functie", i);
    }
    P.divideValues("functie", N);
}

void perf() {
    // P.reset("arbore");
    // perf_1();
    // P.showReport();
    // P.reset("quicksort_h");
    // perf_2();
    P.reset("quicksort_h-optim");
    perf_3();
    P.showReport();
}

void demo_arbore() {
    NodeT *root = nullptr;
    int n = 10;
    int v[n];
    FillRandomArray(v, n, 10, 50000, true);
    for (int i = 0; i < n; i++)
        insert(root, v[i], root);
    // pp(root, 0);
    Operation ops = P.createOperation("asd", n);
    inorder_recursiv(root, ops);
    puts("");
    inorder_iterativ(root, ops);
}

void demo_quick() {
    int n = 10;
    int v[n];
    FillRandomArray(v, n);
    for (int i = 0; i < n; i++)
        printf("%d ", v[i]);
    quicksort_h(v, 0, n - 1);
    puts("");
    for (int i = 0; i < n; i++)
        printf("%d ", v[i]);
    puts("");
}

int main() {
    clock_t begin = clock();
    // demo_arbore();
    // demo_quick();
    perf();
    clock_t end = clock();
    printf("%lf\n", (double)(end - begin) / CLOCKS_PER_SEC);
    return 0;
}