/* Sorin Turda, grupa 30222
*
* Interclasare k liste: are complexitate O(n*log(k))
* pentru k = {5, 10, 100} si n intre 100 si 10000 graficele nu difera
* foarte mult pentru ca valorile log5, log10 si log100 sunt apropiate
* pentru n = 10000 si k intre 10 si 500 diferenta dintre doua valori ale lui k consecutive este mare pana
* la k = 200, iar de acolo, diferenta este mica datorita valorii logaritmului
*
*/
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include "../Profiler.h"

#define MAX_SIZE  10000
#define STEP_SIZE 100
#define NR_TESTS 5

Profiler p("merge_lists");

struct node {
    int key;
    node * next;
};

struct heap {
    node *a[MAX_SIZE];
    int n = MAX_SIZE;
    int heap_size;
};
void build_min_heap(heap &, const int);
void build_min_heap_2(heap &, const int);
void min_heapify(heap &, int, const int);
void min_heapify_2(heap &, int, const int);

int parent(int i) {
    return (i - 1) >> 1; // vector indexat de la 0, relatiile sunt diferite
}

int left(int i) {
    return (i << 1) + 1;
}

int right(int i) {
    return (i << 1) + 2;
}

node * create(int key) {
    node *q = new node;
    q->key = key;
    q->next = NULL;
    return q;
}

void print(node *first) {
    int n = 0;
    while (first != NULL) {
        printf("%d ", first->key);
        first = first->next;
        n++;
    }
    printf(": %d\n", n);
}

void add(node *& first, node *&last, int key) {
    node *q = create(key);
    if (last == NULL) {
        last = first = q;
    }
    else {
        last->next = q;
        last = last->next;
    }
}

node *merge_sorted_lists(node *lists[], const int k, const int N) {
    node *ptr = NULL;
    node *last = NULL;
    heap A;
    A.n = N;
    Operation ops5 = p.createOperation("k=5", A.n);
    Operation ops10 = p.createOperation("k=10", A.n);
    Operation ops100 = p.createOperation("k=100", A.n);
    A.heap_size = k;
    for (int i = 0; i < k; i++)
        A.a[i] = lists[i];
    build_min_heap(A, k);

    // puts("err");
    while (A.heap_size > 0) { // practic ar merge cand e un singur element in heap deoarece doar
        if (last != NULL) {    // din el pot fi celelalte noduri care sunt DEJA legate
            last->next = A.a[0];
            switch (k) {
            case 5: ops5.count();
                break;
            case 10: ops10.count();
                break;
            case 100: ops100.count();
                break;
            }
        }
        else
            ptr = A.a[0];
        switch (k) {
        case 5: ops5.count();
            break;
        case 10: ops10.count();
            break;
        case 100: ops100.count();
            break;
        }
        last = A.a[0];
        A.a[0] = A.a[0]->next;
        if (A.a[0] == NULL) {
            switch (k) {
            case 5: ops5.count();
                break;
            case 10: ops10.count();
                break;
            case 100: ops100.count();
                break;
            }
            A.heap_size--;
            A.a[0] = A.a[A.heap_size];
        }
        if (A.heap_size > 1)
            min_heapify(A, 0, k);
    }
    return ptr;
}

node *merge_sorted_lists_2(node *lists[], const int k, const int N) {
    node *ptr = NULL;
    node *last = NULL;
    heap A;
    A.n = N;
    Operation ops = p.createOperation("cazul 2 n-10000", k);
    A.heap_size = k;
    for (int i = 0; i < k; i++)
        A.a[i] = lists[i];
    build_min_heap_2(A, k);

    while (A.heap_size > 0) { // practic ar merge cand e un singur element in heap deoarece doar
        if (last != NULL) {    // din el pot fi celelalte noduri care sunt DEJA legate
            last->next = A.a[0];
            ops.count();
        }
        else
            ptr = A.a[0];
        ops.count();
        last = A.a[0];
        A.a[0] = A.a[0]->next;
        if (A.a[0] == NULL) {
            ops.count();
            A.heap_size--;
            A.a[0] = A.a[A.heap_size];
        }
        if (A.heap_size > 1)
            min_heapify_2(A, 0, k);
    }
    return ptr;
}

void min_heapify(heap &A, int i, const int k) {
    Operation ops5 = p.createOperation("k=5", A.n);
    Operation ops10 = p.createOperation("k=10", A.n);
    Operation ops100 = p.createOperation("k=100", A.n);
    int l = left(i);
    int r = right(i);
    int smallest = i;
    if (l < A.heap_size) {
        if (k == 5)
            ops5.count();
        else if (k == 10)
            ops10.count();
        else ops100.count();
        if (A.a[l]->key < A.a[i]->key)
            smallest = l;
    }
    if (r < A.heap_size) {
        if (k == 5)
            ops5.count();
        else if (k == 10)
            ops10.count();
        else ops100.count();
        if (A.a[r]->key < A.a[smallest]->key)
            smallest = r;
    }
    if (smallest != i) {
        if (k == 5)
            ops5.count(3);
        else if (k == 10)
            ops10.count(3);
        else ops100.count(3);
        node *aux = A.a[i];
        A.a[i] = A.a[smallest];
        A.a[smallest] = aux;
        min_heapify(A, smallest, k);
    }
}

void min_heapify_2(heap &A, int i, const int k) {
    Operation ops = p.createOperation("cazul 2 n-10000", k);
    int l = left(i);
    int r = right(i);
    int smallest = i;
    if (l < A.heap_size) {
        ops.count();
        if (A.a[l]->key < A.a[i]->key)
            smallest = l;
    }
    if (r < A.heap_size) {
        ops.count();
        if (A.a[r]->key < A.a[smallest]->key)
            smallest = r;
    }
    if (smallest != i) {
        ops.count(3);
        node *aux = A.a[i];
        A.a[i] = A.a[smallest];
        A.a[smallest] = aux;
        min_heapify_2(A, smallest, k);
    }
}

void build_min_heap(heap &A, const int k) {
    for (int i = A.heap_size / 2 - 1; i >= 0; i--)
        min_heapify(A, i, k);
}

void build_min_heap_2(heap &A, const int k) {
    for (int i = A.heap_size / 2 - 1; i >= 0; i--)
        min_heapify_2(A, i, k);
}

void demo() {
    node *f1 = NULL, *l1 = NULL;
    node *f2 = NULL, *l2 = NULL;
    node *f3 = NULL, *l3 = NULL;
    node *f4 = NULL, *l4 = NULL;
    const int n = 5;
    int v[n];
    FillRandomArray(v, n, 10, 200, false, ASCENDING);
    for (int i = 0; i < n; i++)
        add(f1, l1, v[i]);
    FillRandomArray(v, n, 10, 200, false, ASCENDING);
    for (int i = 0; i < n; i++)
        add(f2, l2, v[i]);
    FillRandomArray(v, n, 10, 200, false, ASCENDING);
    for (int i = 0; i < n; i++)
        add(f3, l3, v[i]);
    for (int i = 0; i < n; i++)
        add(f4, l4, v[i]);
    l1 = l2 = l3 = l4 = NULL;
    delete l1, l2, l3, l4;
    print(f1);
    print(f2);
    print(f3);
    print(f4);
    node *L[] = {f1, f2, f3, f4};
    // printf("%d\n",f1->key);
    int k = sizeof(L) / sizeof(L[0]);
    print(merge_sorted_lists(L, k, 0));
    // print(f2);
}

void create_list(int *v, int n, node *& first, node *& last) {
    for (int i = 0; i < n; i++)
        add(first, last, v[i]);
}

void generate_lists(node * lists[], const int k, const int n) {
    int size = n / k;
    int v[size + n % k];
    for (int j = 0; j < k - 1; j++) {
        FillRandomArray(v, size, 10, 50000, false, ASCENDING);
        node *first = NULL, *last = NULL;
        create_list(v, size, first, last);
        lists[j] = first;
    }
    node *first = NULL, *last = NULL;
    if (n % k == 0) {
        FillRandomArray(v, size, 10, 50000, false, ASCENDING);
        create_list(v, size, first, last);
    }
    else {
        FillRandomArray(v, size + n % k, 10, 50000, false, ASCENDING);
        create_list(v, size + n % k, first, last);
    }
    lists[k - 1] = first;
}

void perf_1() {
    const int K[] = {5, 10, 100};
    const int k = sizeof(K) / sizeof(K[0]);
    node *ptr = NULL;
    node * lists[100];
    int v[MAX_SIZE];
    for (int j = 0; j < k; j++)
        for (int n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE)
            for (int test = 0; test < NR_TESTS; test++) {
                generate_lists(lists, K[j], n);
                ptr = merge_sorted_lists(lists, K[j], n);
            }
    // print(lists[0]);
    p.divideValues("k=5", NR_TESTS);
    p.divideValues("k=10", NR_TESTS);
    p.divideValues("k=100", NR_TESTS);
    p.createGroup("k constant", "k=5", "k=10", "k=100");
    // p.createGroup("k={5,10}", "k=5", "k=10");
    p.showReport();
}

void perf_2() {
    const int NR_LISTS = 500;

    for (int k = 10; k <= NR_LISTS; k += 10) {
        for (int test = 0; test < NR_TESTS; test++) {
            node * lists[k];
            generate_lists(lists, k, MAX_SIZE);
            node *ptr = merge_sorted_lists_2(lists, k, MAX_SIZE);
        }
    }
    p.divideValues("cazul 2 n-10000", NR_TESTS);
    p.showReport();
}

void perf() {
    p.reset("k-constant");
    perf_1();
    p.reset("n-constant");
    perf_2();
}

int main() {
    srand(time(NULL));
    clock_t begin = clock();
    demo();
    // perf();
    clock_t end = clock();
    printf("%lf\n", (double)(end - begin) / CLOCKS_PER_SEC);
    return 0;
}
