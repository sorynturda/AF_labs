#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include "../Profiler.h"

#define MAX_SIZE  10000
#define STEP_SIZE 100
#define NR_TESTS 5
#define N 10007
#define a 5
#define b 19
#define c 29

Profiler p("hashtable");

struct Entry {
    int id;
    bool free = true;
    bool used = false;
    char name[30];
};

int h(int k, int i) {
    int p = (((k % N) * (a % N)) % N + ((((i % N) * (i % N)) % N) * (a % N))) % N;
    p = (p % N + c % N) % N;
    return p;
}

int hash_insert(Entry T[], int key) {
    int i = 0, q;
    do {
        q = h(key, i);
        if (T[q].free == true) {
            T[q].id = key;
            T[q].used = true;
            T[q].free = false;
            return q;
        }
        else
            i++;
    } while (i < N);
    puts("hash table overflow");
    exit(-1);
}

int hash_search(Entry T[], int key) {
    int i = 0, q;
    do {
        q = h(key, i);
        if (T[q].free == false && T[q].id == key)
            return q;
        else
            i++;
    } while (T[q].used == true && i < N);
    return -1;
}

void hash_delete(Entry T[], int key) {
    int q = hash_search(T, key);
    if (q == -1)
        return;
    T[q].free = true;
}
void demo() {
    Entry T[N];
    int pos = hash_insert(T, 10008);
    int pos1 = hash_insert(T, 1);
    printf("%d\n", pos);
    printf("%d\n", pos1);
    printf("%d\n", hash_search(T, 10008));
    // hash_delete(T, 1);
    // hash_delete(T, 10008);
    printf("%d\n", T[hash_search(T, 1)].id);
    printf("%d\n", T[hash_search(T, 10008)].id);
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
