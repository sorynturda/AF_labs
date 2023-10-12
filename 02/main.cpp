#include <stdio.h>

void print(int *, int);

int parent(int i) {
    return ((i - 1) >> 1); //vector indexat de la 0, relatiile sunt diferite
}

int left(int i) {
    return (i << 1) + 1; //fiu stang
}

int right(int i) {
    return (i << 1) + 2;
}


void max_heapify(int a[], int i, int n) {
    int l = left(i);
    int r = right(i);
    int largest;
    if (l < n && a[l] > a[i]) //aleg maximul dintre parinte si cei 2 fii
        largest = l;
    else largest = i;
    if (r < n && a[r] > a[largest])
        largest = r;
    if (largest != i) {
        int aux = a[i];
        a[i] = a[largest];
        a[largest] = aux;
        max_heapify(a, largest, n);
    }
}

void build_max_heap(int a[], int n) {
    for (int i = n / 2 - 1; i >= 0; i--)
        max_heapify(a, i, n);
}

void heapsort(int a[], int n) {
    for (int i = n - 1; i > 0; i--) {
        int aux = a[i];
        a[i] = a[0];
        a[0] = aux;
        max_heapify(a, 0, i);
    }
}

void print(int a[], int n) {
    // printf("%d\n", n);
    for (int i = 0; i < n; i++)
        printf("%d ", *(a + i));
    puts("");
}

void demo() {
    int a[] = {3, 5, 4, 2, 6, 10, 9, 12, 15, 1};
    int n = sizeof(a) / sizeof(a[0]);
    build_max_heap(a, n);
    print(a, n);
    heapsort(a, n);
    print(a, n);
}

int main() {
    demo();
    return 0;
}