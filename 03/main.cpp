/* Sorin Turda, grupa 30222
* Heapsort are complexitate O(n*logn) daca heapul este construit cu metoda build bottom up
* Quicksort are complexitatea O(n^2) pe cazul cand vectorul este sortat descrescator (cazul defavorabil)
* pe cazul mediu (un vector cu elementele in ordine arbitrara) quicksort este mai rapid decat heapsort
*
* bubble sort recursiv vs iterativ: ambii algoritmi efectueaza acelasi numar de operatii
* din grafic rezulta ca bubble sort interativ este putin mai rapid decat bubble sort recursiv
*/
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include "/home/sorin/Documents/facultate/AF_labs/Profiler.h"

#define MAX_SIZE 10000
#define STEP_SIZE 100
#define NR_TESTS 100

Profiler p("sortari nlogn");

struct heap {
    int a[MAX_SIZE];
    int heap_size = 0;
    int n = MAX_SIZE;
};

int partition_r(int *, int, int, const int);

void copy(int *a, int *b, int n) {
    for (int i = 0; i < n; i++)
        a[i] = b[i];
}

void copy(heap &A, int *v) {
    for (int i = 0; i < A.n; i++)
        A.a[i] = v[i];
}

int left(int i) {
    return (i << 1) + 1;
}

int right(int i) {
    return (i << 1) + 2;
}

void printt_array(int a[], int n) {
    for (int i = 0; i < n; i++)
        printf("%d ", a[i]);
    puts("");
}

void printt_array(heap A) {
    for (int i = 0; i < A.n; i++)
        printf("%d ", A.a[i]);
    puts("");
}

void bubble_sort(int v[], int n) {
    Operation op = p.createOperation("Total bubble sort", n);
    bool flag = true;
    for (int j = n - 2; j >= 0 && flag; j--) {
        flag = false;
        for (int i = 0; i <= j; i++) {
            op.count();
            if (v[i] > v[i + 1]) {
                op.count(3);
                int aux = v[i];
                v[i] = v[i + 1];
                v[i + 1] = aux;
                flag = true;
            }
        }
    }
}

void bubble_sort_recursiv(int v[], int n, const int N) {
    if (n == 1)
        return;
    Operation op = p.createOperation("Total bubble sort recursiv", N);
    bool flag = false;
    for (int i = 0; i < n - 1; i++) {
        op.count();
        if (v[i] > v[i + 1]) {
            op.count(3);
            std::swap(v[i], v[i + 1]);
            flag = true;
        }
    }
    if (flag == true)
        bubble_sort_recursiv(v, n - 1, N);
}

void max_heapify(heap &A, int i) {
    Operation compBUp = p.createOperation("Comparari heapsort", A.n);
    Operation atrBUp = p.createOperation("Atribuiri heapsort", A.n);
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
    Operation compBUp = p.createOperation("Comparari heapsort", A.n);
    Operation atrBUp = p.createOperation("Atribuiri heapsort", A.n);
    A.heap_size = A.n;
    atrBUp.count();
    for (int i = A.heap_size / 2 - 1; i >= 0; i--)
        max_heapify(A, i);
}

int randomized_partition(int a[], int left, int right, const int n) {
    Operation comprQs = p.createOperation("Comparari quicksort randomizat", n);
    Operation atrrQs = p.createOperation("Atribuiri quicksort randomizat", n);
    int i = rand() % (right - left + 1) + left;
    std::swap(a[i], a[right]);
    atrrQs.count(3);
    return partition_r(a, left, right, n);
}

void randomized_quicksort(int a[], int left, int right, const int n) {
    srand(time(NULL));
    if (left >= right)
        return;
    int q = randomized_partition(a, left, right, n);
    randomized_quicksort(a, left, q - 1, n);
    randomized_quicksort(a, q + 1, right, n);
}

int partition_r(int a[], int left, int right, const int n) {
    Operation comprQs = p.createOperation("Comparari quicksort randomizat", n);
    Operation atrrQs = p.createOperation("Atribuiri quicksort randomizat", n);
    int x = a[right]; // pivot elementul cel mai din dreapta
    atrrQs.count();
    int i = left - 1;
    for (int j = left; j <= right - 1; j++) {
        comprQs.count();
        if (a[j] <= x) {
            i++;
            atrrQs.count(3);
            std::swap(a[i], a[j]);
        }
    }
    atrrQs.count();
    std::swap(a[i + 1], a[right]); // punem pivotul la locul lui
    return i + 1; // pozitia unde e amplasat pivotul
}

int partition(int a[], int left, int right, const int n) {
    Operation compQs = p.createOperation("Comparari quicksort", n);
    Operation atrQs = p.createOperation("Atribuiri quicksort", n);
    int x = a[right]; // pivot elementul cel mai din dreapta
    atrQs.count();
    int i = left - 1;
    for (int j = left; j <= right - 1; j++) {
        compQs.count();
        if (a[j] <= x) {
            i++;
            atrQs.count(3);
            std::swap(a[i], a[j]);
        }
    }
    atrQs.count();
    std::swap(a[i + 1], a[right]); // punem pivotul la locul lui
    return i + 1; // pozitia unde e amplasat pivotul
}

int partition_med(int a[], int left, int right, const int n) {
    Operation compQs = p.createOperation("Comparari quicksort", n);
    Operation atrQs = p.createOperation("Atribuiri quicksort", n);
    int mid = left + ((right - left) >> 1);
    int x = a[mid]; // pivot elementul din mijloc
    std::swap(a[mid], a[right]);
    atrQs.count();
    int i = left - 1;
    for (int j = left; j <= right - 1; j++) {
        compQs.count();
        if (a[j] <= x) {
            i++;
            atrQs.count(3);
            std::swap(a[i], a[j]);
        }
    }
    atrQs.count();
    std::swap(a[i + 1], a[right]); // punem pivotul la locul lui
    return i + 1; // pozitia unde e amplasat pivotul
}

void quicksort(int a[], int left, int right, const int n) {
    if (left >= right)
        return;
    int q = partition(a, left, right, n);
    quicksort(a, left, q - 1, n);
    quicksort(a, q + 1, right, n);
}

void quicksort_med(int a[], int left, int right, const int n) {
    if (left >= right)
        return;
    int q = partition_med(a, left, right, n);
    quicksort_med(a, left, q - 1, n);
    quicksort_med(a, q + 1, right, n);
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

int partition_quickselect(int a[], int left, int right) {
    int pivot = rand() % (right - left + 1) + left;
    int x = a[pivot];
    std::swap(a[right], a[pivot]);
    int i = left - 1;
    for (int j = left; j <= right - 1; j++)
        if (a[j] <= x)
            std::swap(a[++i], a[j]);
    std::swap(a[i + 1], a[right]);
    return i + 1;
}

int quickselect(int a[], int left, int right, int k) {
    int index = partition_quickselect(a, left, right);
    if (k == index)
        return a[k];
    else if (k < index)
        return quickselect(a, left, index - 1, k);
    return quickselect(a, index + 1, right, k);
}

void demo() {
    int a[] = { -1, 12, 1, 7, 3, 2, 8, 6, 4, 123};
    int b[] = { -1, 12, 1, 7, 3, 2, 8, 6, 4, 123};
    int n = sizeof(a) / sizeof(a[0]);
    quicksort(b, 0, n - 1, n);
    printt_array(b, n);
    heap A;
    A.n = n;
    copy(A, a);
    heapsort(A);
    printt_array(A);
}

void perf(int order) {
    int v[MAX_SIZE], a[MAX_SIZE];
    for (int n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE)
        for (int test = 0; test < NR_TESTS; test++) {
            heap A;
            FillRandomArray(v, n, 10, 50000, false, order);
            copy(a, v, n);
            if (order != DESCENDING)
                quicksort(a, 0, n - 1, n);
            else
                quicksort_med(a, 0, n - 1, n);
            copy(A, v);
            A.heap_size = n;
            A.n = n;
            heapsort(A);
            if (order != UNSORTED)
                break;
        }
    if (order == UNSORTED) {
        p.divideValues("Comparari quicksort", NR_TESTS);
        // p.divideValues("Comparari quicksort randomizat", NR_TESTS);
        p.divideValues("Atribuiri quicksort", NR_TESTS);
        // p.divideValues("Atribuiri quicksort randomizat", NR_TESTS);
        p.divideValues("Comparari heapsort", NR_TESTS);
        p.divideValues("Atribuiri heapsort", NR_TESTS);
    }
    p.addSeries("Total quicksort", "Comparari quicksort", "Atribuiri quicksort");
    // p.addSeries("Total quicksort randomizat", "Comparari quicksort randomizat", "Atribuiri quicksort randomizat");
    p.addSeries("Total heapsort", "Comparari heapsort", "Atribuiri heapsort");
    p.createGroup("Atribuiri", "Atribuiri quicksort", "Atribuiri heapsort");
    p.createGroup("Comparari", "Comparari quicksort", "Comparari heapsort");
    p.createGroup("Total", "Total quicksort", "Total heapsort");
    if (order == ASCENDING) {
        p.createGroup("Heapsort comparari", "Comparari heapsort");
        p.createGroup("Heapsort atribuiri", "Atribuiri heapsort");
        p.createGroup("Heapsort total", "Total heapsort");
    }
    p.showReport();
}

void perf_bubble(int order) {
    int v[MAX_SIZE], a[MAX_SIZE], b[MAX_SIZE];
    for (int n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE) {
        FillRandomArray(v, n, 10, 50000, false, order);
        p.startTimer("bubble sort time", n);
        for (int test = 0; test < NR_TESTS; test++) {
            copy(b, v, n);
            bubble_sort(b, n);
        }
        p.stopTimer("bubble sort time", n);
        p.startTimer("bubble sort recursiv time", n);
        for (int test = 0; test < NR_TESTS; test++) {
            copy(a, v, n);
            bubble_sort_recursiv(a, n, n);
        }
        p.stopTimer("bubble sort recursiv time", n);

        for (int test = 0; test < NR_TESTS; test++) {
            copy(b, v, n);
            bubble_sort(b, n);
        }
        for (int test = 0; test < NR_TESTS; test++) {
            copy(a, v, n);
            bubble_sort_recursiv(a, n, n);
        }
    }
    p.divideValues("Total bubble sort", NR_TESTS);
    p.divideValues("Total bubble sort recursiv", NR_TESTS);
    p.divideValues("bubble sort recursiv time", NR_TESTS);
    p.divideValues("bubble sort time", NR_TESTS);
    p.createGroup("Total", "Total bubble sort", "Total bubble sort recursiv");
    p.createGroup("Timp", "bubble sort recursiv time", "bubble sort time");
    p.showReport();
}

void perf_all() {
    // p.reset("defavorabil");
    // perf(ASCENDING);
    // p.reset("mediu");
    // perf(UNSORTED);
    // p.reset("favorabil");
    // perf(DESCENDING);
    p.reset("buble-sort-mediu");
    perf_bubble(UNSORTED);
}

int main() {
    clock_t begin = clock();
    // demo();
    perf_all();
    clock_t end = clock();
    printf("%lf\n", (double)(end - begin) / CLOCKS_PER_SEC);
    return 0;
}
