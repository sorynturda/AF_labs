#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include "/home/sorin/Documents/facultate/AF_labs/Profiler.h"

#define MAX_SIZE 10000
#define STEP_SIZE 100
#define NR_TESTS 5

Profiler p("liste");

struct node {
    int key;
    node * next;
};

struct heap {
    node *a[50];
    int n = 50;
    int heap_size;
};

node * create(int key) {
    node *q = new node;
    q->key = key;
    q->next = NULL;
    return q;
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

void print(node *first) {
    while (first != NULL) {
        printf("%d ", first->key);
        first = first->next;
    }
    puts("");
}

node *merge(node *&List1, node *&List2) { // interclasare 2 liste
    if (List1 == NULL && List2 == NULL)
        return NULL;
    if (List1 == NULL)
        return List2;
    if (List2 == NULL)
        return List1;
    node *list = List1;
    if (List1->key > List2->key) {
        list = List2;
        List2 = List2->next;
    }
    else
        List1 = List1->next;
    node *curr = list;
    while (List1 && List2) {
        if (List1->key <= List2->key) {
            curr->next = List1;
            List1 = List1->next;
        }
        else {
            curr->next = List2;
            List2 = List2->next;
        }
        curr = curr->next;
    }
    if (!List1)
        curr->next = List2;
    else
        curr->next = List1;
    return list;
}

void min_heap_insert(heap &A, node *nod) {
    if (A.heap_size == A.n) {
        puts("heap overflow")l
        exit(-1);
    }
    A.heap_size++;
    A.a[heap_size] = nod;
    int i = A.heap_size;
    while (i > 0) {
        if (A.a[(i - 1) >> 1]->key > A.a[i]->key) {
            node *aux = A.a[i];
            A.a[i] = A.a[(i - 1) >> 1];
            A.a[(i - 1) >> 1] = aux;
            i = (i - 1) >> 1;
        }
        else break;
    }
}

void min_heapify(heap &A, int i) {
    int l = i << 1 + 1;
    int r = i << 2 + 2;
    int smallest = i;
    if (l < A.heap_size) {
        if (A.a[l]->key < A.a[i]->key)
            smallest = l;
    }
    if (r < A.heap_size) {
        if (A.a[r]->key < A.a[smallest]->key)
            smallest = r;
    }
    if (smallest != i) {
        node *aux = A.a[i];
        A.a[i] = A.a[smallest];
        A.a[smallest] = aux;
    }
}

void bottom_up(heap &A) {
    A.heap_size = A.n;
    for (int i = A.n / 2 - 1; i >= 0; i--)
        min_heapify(A, i);
}

void demo() {
    node *f1 = NULL, *l1 = NULL;
    node *f2 = NULL, *l2 = NULL;
    const int n = 5;
    int v[n];
    FillRandomArray(v, n, 10, 50, false, ASCENDING);
    for (int i = 0; i < n; i++)
        add(f1, l1, v[i]);
    FillRandomArray(v, n, 10, 50, false, ASCENDING);
    for (int i = 0; i < n; i++)
        add(f2, l2, v[i]);
    l1 = l2 = NULL;
    delete l1, l2;
    print(merge(f1, f2));
    print(f1);
    print(f2);
}

int main() {
    clock_t begin = clock();

    demo();

    clock_t end = clock();
    printf("%lf\n", (double)(end - begin) / CLOCKS_PER_SEC);
    return 0;
}
