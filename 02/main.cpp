#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_SIZE 50000

void print(int *, int);
void max_heapify(int *, int, int);

int parent(int i) {
    return ((i - 1) >> 1); // vector indexat de la 0, relatiile sunt diferite
}

int left(int i) {
    return (i << 1) + 1;
}

int right(int i) {
    return (i << 1) + 2;
}

int max_heap_maximum(int a[], int heap_size) {
    if (heap_size < 1) {
        printf("heap underflow\n");
        exit(-1);
    }
    return a[0];
}

int max_heap_extract_max(int a[], int &heap_size) {
    int max = max_heap_maximum(a, heap_size);
    a[0] = a[--heap_size]; // in radacina merge ultimul element din vector
    max_heapify(a, 0, heap_size);
    return max;
}

void max_heap_insert(int a[], int x, int heap_size, const int N){
    if (heap_size == N){
        puts("heap overflow");
        exit(-1);
    }
    heap_size++;
    // TO_DO
}

void max_heapify(int a[], int i, int heap_size) {
    int l = left(i);
    int r = right(i);
    int largest;
    if (l < heap_size && a[l] > a[i]) // aleg maximul dintre parinte si cei 2 fii
        largest = l;
    else largest = i;
    if (r < heap_size && a[r] > a[largest])
        largest = r;
    if (largest != i) {
        int aux = a[i];
        a[i] = a[largest];
        a[largest] = aux;
        max_heapify(a, largest, heap_size);
    }
}

void build_max_heap(int a[], int heap_size) {
    for (int i = heap_size / 2 - 1; i >= 0; i--)
        max_heapify(a, i, heap_size);
}

void heapsort(int a[], int heap_size) {
    for (int i = heap_size - 1; i > 0; i--) {
        int aux = a[i];
        a[i] = a[0];
        a[0] = aux;
        max_heapify(a, 0, i);
    }
}

void print(int a[], int heap_size) {
    // printf("%d\n", heap_size);
    for (int i = 0; i < heap_size; i++)
        printf("%d ", *(a + i));
    puts("");
}

void demo() {
    int a[] = {3, 5, 4, 2, 6, 10, 9, 12, 15, 1};
    int heap_size = sizeof(a) / sizeof(a[0]);
    const int N = heap_size; // dimensiune vector
    build_max_heap(a, heap_size);
    // print(a, heap_size);
    // heapsort(a, heap_size);
    // print(a, heap_size);
    printf("%d\n", max_heap_extract_max(a, heap_size));
    print(a, heap_size);
}

int main() {
    demo();
    return 0;
}