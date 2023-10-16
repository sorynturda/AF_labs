/*
* Sorin Turda, grupa 30222
* pe cazul mediu statistic construirea heap-ului prin tehnica bottom up este mai rapida de cat tehnica top down, la fel si pe
* cazul defavorabil unde construirea heap-ului prin tehnica bottom up este aproape de 10 ori mai rapida decat cealalta
*
* functia max_heapifya are complexitate O(log(n))
*
* complexitatea algoritmului de construire bottom up este O(n) a algoritmului top down este O(n * log(n))
* tehnica de construire bottom up de construire a heapului  se foloseste pentru heapsort care are complexitate O(n * log(n))
* se foloseste tehnica top down pentru construirea unui heap pentru cozi de prioritate
*/
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include "../Profiler.h"

#define MAX_SIZE 10000
#define STEP_SIZE 100
#define NR_TESTS 5

Profiler p("heap");


struct heap {
    int a[MAX_SIZE];
    int heap_size = 0;
    int n = MAX_SIZE;
};

void max_heapify(heap&, int);
void copy(heap&, int*);

int parent(int i) {
    return (i - 1) >> 1; // vector indexat de la 0, relatiile sunt diferite
}

int left(int i) {
    return (i << 1) + 1;
}

int right(int i) {
    return (i << 1) + 2;
}

int max_heap_maximum(heap A) {
    if (A.heap_size < 1) {
        printf("heap underflow\n");
        exit(-1);
    }
    return A.a[0];
}

int max_heap_extract_max(heap &A) {
    int max = max_heap_maximum(A);
    A.a[0] = A.a[--A.heap_size]; // in radacina merge ultimul element din vector
    max_heapify(A, 0);
    return max;
}

void max_heap_insert(heap &A, int x) {
    Operation compTD = p.createOperation("Comparari top down", A.n);
    Operation atrTD = p.createOperation("Atribuiri top down", A.n);
    if (A.heap_size == A.n) {
        puts("heap overflow");
        exit(-1);
    }
    A.heap_size++;
    atrTD.count(2);
    A.a[A.heap_size - 1] = x;
    int i = A.heap_size;
    while (i > 0) {
        compTD.count();
        if (A.a[parent(i)] < A.a[i]) {
            atrTD.count(3);
            int aux = A.a[i];
            A.a[i] = A.a[parent(i)];
            A.a[parent(i)] = aux;
            i = parent(i);
        }
        else break;
    }
}

void max_heapify(heap &A, int i) {
    Operation compBUp = p.createOperation("Comparari bottom up", A.n);
    Operation atrBUp = p.createOperation("Atribuiri bottom up", A.n);
    int l = left(i);
    int r = right(i);
    int largest = i;
    if (l < A.heap_size) {
        compBUp.count();
        if (A.a[l] > A.a[i]) {
            largest = l;
        }
        else {
            largest = i;
        }
    }
    if (r < A.heap_size) {
        compBUp.count();
        if (A.a[r] > A.a[largest])
            largest = r;
    }
    if (largest != i) {
        atrBUp.count(3);
        int aux = A.a[i];
        A.a[i] = A.a[largest];
        A.a[largest] = aux;
        max_heapify(A, largest);
    }
}

void build_max_heap_bottom_up(heap &A) {
    Operation compBUp = p.createOperation("Comparari bottom up", A.n);
    Operation atrBUp = p.createOperation("Atribuiri bottom up", A.n);
    A.heap_size = A.n;
    atrBUp.count();
    for (int i = A.heap_size / 2 - 1; i >= 0; i--)
        max_heapify(A, i);
}

void build_max_heap_top_down(heap &A) {
    Operation compTD = p.createOperation("Comparari top down", A.n);
    Operation atrTD = p.createOperation("Atribuiri top down", A.n);
    A.heap_size = 1;
    atrTD.count();
    for (int i = 1; i < A.n; i++)
        max_heap_insert(A, A.a[i]);
}

void heapsort(heap &A) {
    build_max_heap_bottom_up(A);
    int n = A.heap_size;
    for (int i = n - 1; i >= 0; i--) {
        int aux = A.a[i];
        A.a[i] = A.a[0];
        A.a[0] = aux;
        A.heap_size--;
        max_heapify(A, 0);
    }
}

void print_heap(heap A) {
    for (int i = 0; i < A.heap_size; i++)
        printf("%d ", *(A.a + i));
    puts("");
}

void ch(heap A){
    for(int i = 0; i < A.heap_size/2; i++){
        printf("%d: ", A.a[i]);
        if(left(i) < A.heap_size)
            printf("%d ", A.a[left(i)]);
        if(right(i) < A.heap_size)
            printf("%d ", A.a[right(i)]);
        puts("");
    }
}

void demo1() {
    heap A;
    int v[] = {3, 5, 4, 2, 6, 10, 9, 12, 15, 1};
    int n = sizeof(v) / sizeof(v[0]);
    A.n = n;
    copy(A, v);
    A.heap_size = n;
    build_max_heap_bottom_up(A);
    print_heap(A);
    ch(A);
    // printf("%d\n", max_heap_extract_max(A));
    // max_heap_insert(A,-5);
    heapsort(A);
    for (int i = 0; i < n; i++)
        printf("%d ", A.a[i]);
}

void demo2() {
    heap A;
    int v[] = {3, 5, 4, 2, 6, 10, 9, 12, 15, 1};
    int n = sizeof(v) / sizeof(v[0]);
    A.n = n;
    copy(A, v);
    build_max_heap_top_down(A);
    ch(A);
    print_heap(A);
}

void copy(heap &A, int v[]) {
    for (int i = 0; i < A.n; i++)
        A.a[i] = v[i];
}

void perf(int order) {
    int v[MAX_SIZE];
    for (int n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE)
        for (int i = 0; i < NR_TESTS; i++) {
            heap A, B;
            FillRandomArray(v, n, 10, 50000, false, order); //vezi Profiler
            A.n = n;
            B.n = n;
            copy(A, v);
            copy(B, v);
            // print_heap(A);
            build_max_heap_bottom_up(A);
            build_max_heap_top_down(B);
            if (order == ASCENDING)
                break;
        }

    if (order != ASCENDING) {
        p.divideValues("Comparari bottom up", NR_TESTS);
        p.divideValues("Atribuiri bottom up", NR_TESTS);
    }
    p.addSeries("Total bottom up", "Comparari bottom up", "Atribuiri bottom up");
    if (order != ASCENDING) {
        p.divideValues("Comparari top down", NR_TESTS);
        p.divideValues("Atribuiri top down", NR_TESTS);
    }
    p.addSeries("Total top down", "Comparari top down", "Atribuiri top down");

    p.createGroup("Comparari", "Comparari bottom up", "Comparari top down");
    p.createGroup("Atribuiri", "Atribuiri bottom up", "Atribuiri top down");
    p.createGroup("Total", "Total bottom up", "Total top down");
    p.showReport();
}

int main() {
    clock_t begin = clock();
    // demo2();
    // demo1();
    p.reset("mediu");
    perf(UNSORTED);
    p.reset("defavorabil");
    perf(ASCENDING);
    clock_t end = clock();
    printf("\n%lf\n", (double)(end - begin) / CLOCKS_PER_SEC);
    return 0;
}