/*
*
* Sorin Turda, 30222
* pe cazul mediu, insertion sort are cea mai buna complexitate
* si bubble sort are cea mai slaba complexitate
* cei 3 algoritmi au complexitate O(n^2)
* pe cazul defavorabil selection sort le bate pe celelalte doua la complexitate
*
*/
#include <stdio.h>
#include "Profiler.h"
#include <time.h>

#define NMAX 10000
#define STEP_SIZE 100
#define NR_TESTS 5

Profiler p("sort");

void bubble_sort(int v[], int n) {
    Operation opComp = p.createOperation("bs-comp", n);
    Operation opAttr = p.createOperation("bs-attr", n);
    bool flag = true;
    for (int j = n - 2; j >= 0 && flag; j--) {
        flag = false;
        for (int i = 0; i <= j; i++) {
            opComp.count();
            if (v[i] > v[i + 1]) {
                opAttr.count(3);
                int aux = v[i];
                v[i] = v[i + 1];
                v[i + 1] = aux;
                flag = true;
            }
        }
    }
}

void insertion_sort(int v[], int n) {
    //e un algoritm stabil
    Operation opComp = p.createOperation("is-comp", n);
    Operation opAttr = p.createOperation("is-attr", n);
    for (int i = 1; i < n; i++) {
        int x = v[i];
        opAttr.count();
        int st = 0, dr = i - 1, mid;
        while (st <= dr) {
            mid = st + (dr - st) / 2;
            opComp.count();
            if (v[mid] <= x)
                st = mid + 1;
            else
                dr = mid - 1;
        }
        // printf("%d\n", st);
        for (int j = i - 1; j >= st - 1; j--) {
            opAttr.count();
            v[j + 1] = v[j];
        }
        opAttr.count();
        v[st] = x;
    }
}

void selection_sort(int v[], int n) {
    Operation opComp = p.createOperation("ss-comp", n);
    Operation opAttr = p.createOperation("ss-attr", n);
    for (int i = 0; i < n - 1; i++) {
        int pmin = i;
        for (int j = i + 1; j < n ; j++) {
            opComp.count();
            if (v[j] < v[pmin])
                pmin = j;
        }
        if (pmin != i) {
            opAttr.count(3);
            int aux = v[i];
            v[i] = v[pmin];
            v[pmin] = aux;
        }
    }
}

void print_array(int v[], int n) {
    for (int i = 0; i < n; i++)
        printf("%d ", v[i]);
    puts("");
}

void copy(int *a, int *b, int n) {
    for (int i = 0; i < n; i++)
        a[i] = b[i];
}

void demo() {
    int v[] = {6, 5, 1, 7, 9, 12, 2, 4};
    int n = sizeof(v) / sizeof(v[0]);
    int a[n], b[n];
    copy(a, v, n);
    copy(b, v, n);
    bubble_sort(v, n);
    print_array(v, n);
    insertion_sort(a, n);
    print_array(a, n);
    selection_sort(b, n);
    print_array(b, n);
}

void perf(int order) {
    int v[NMAX], a[NMAX];
    int n;
    for (n = STEP_SIZE; n <= NMAX; n += STEP_SIZE) {
        for (int test = 0; test < NR_TESTS; test++) {
            FillRandomArray(v, n, 10, 50000, false, order); //vezi Profiler
            copy(a, v, n);
            bubble_sort(a, n);
            copy(a, v, n);
            insertion_sort(a, n);
            copy(a, v, n);
            selection_sort(a, n);
            if (order != UNSORTED)
                break;
        }
    }
    if (order == UNSORTED) {
        p.divideValues("bs-comp", NR_TESTS);
        p.divideValues("bs-attr", NR_TESTS);

        p.divideValues("is-comp", NR_TESTS);
        p.divideValues("is-attr", NR_TESTS);
        
        p.divideValues("ss-comp", NR_TESTS);
        p.divideValues("ss-attr", NR_TESTS);
    }
    p.addSeries("bs", "bs-attr", "bs-comp");
    p.addSeries("is", "is-attr", "is-comp");
    p.addSeries("ss", "ss-attr", "ss-comp");

    p.createGroup("attr", "bs-attr", "is-attr", "ss-attr");
    p.createGroup("comp", "bs-comp", "is-comp", "ss-comp");
    p.createGroup("total", "bs", "is", "ss");
    if (order == ASCENDING) {
        p.createGroup("bs-is-comp", "bs-comp", "is-comp");
        p.createGroup("bs-is", "bs", "is");
    }
    else {
        p.createGroup("is-Comp", "is-comp");
        p.createGroup("ss-Attr", "ss-attr");
    }
}

void perf_all() {
    p.reset("favorabil");
    perf(ASCENDING);
    p.reset("mediu");
    perf(UNSORTED);
    p.reset("defavorabil");
    perf(DESCENDING);

    p.showReport(); //afiseaza grafice
}

int main() {
    clock_t  begin = clock();
    // demo();
    //perf(UNSORTED);
    perf_all();
    clock_t end = clock();
    printf("%lf\n", (double)(end - begin) / CLOCKS_PER_SEC);
    return 0;
}