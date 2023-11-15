#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include <unistd.h>
#include "../Profiler.h"

#define MAX_SIZE  40000
#define N 10007 //aici pentru demo
#define M 3000
#define a 7
#define b 19
#define c 29

Profiler p("hashtable");

struct Entry {
    int id = 0;
    bool free = true;
    bool used = false;
    char name[30];
};

struct data {
    int celule_accesate = 0;

};

int h(int k, int i) {
    int p = (((k % N) * (a % N)) % N + ((((i % N) * (i % N)) % N) * (a % N))) % N;
    p = (p % N + c % N) % N;
    return p;
}

void print(Entry T[]) {
    for (int i = 0; i < N; i++)
        printf("%d: %d\n", i, T[i].id);
    puts("");
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
    return -1;
}

int hash_search(Entry T[], int key, data &op) {
    int i = 0, q;
    do {
        q = h(key, i);
        op.celule_accesate++;
        if (T[q].free == false && T[q].id == key)
            return q;
        else
            i++;
    } while (T[q].used == true && i < N);
    return -1;
}

void hash_delete(Entry T[], int key, data &op) {
    int q = hash_search(T, key, op);
    if (q == -1)
        return;
    T[q].free = true;
}

void perf() {
    int array[MAX_SIZE];
    Entry T[N];
    FillRandomArray(array, MAX_SIZE, 1, MAX_SIZE, true, UNSORTED);
    int n = 0;
    double α = 1.0 * n / N;
    for (int i = 0; α < 0.99 && i < N; i++) {
        if (hash_insert(T, array[i]) != -1)
            n++;
        α = 1.0 * n / N;
    }
    data op;
    int cel = 0;
    int v[M];
    // sleep(0.01);
    int gasite = 0;
    while (1) {
        while (M / 2 > gasite) {
            data opp;
            int x = rand() % MAX_SIZE + 1;
            if (hash_search(T, x, opp) != -1) {
                gasite++;
                cel = std::max(cel, opp.celule_accesate);
            }
            op.celule_accesate += opp.celule_accesate;
        }
        if (M / 2 <= gasite)
            break;
        gasite = 0;
        op.celule_accesate = 0;
        // sleep(0.1);
    }
    printf("gasite: %d\naccesate: %d\nmaxim %d\n", gasite, op.celule_accesate, cel);
    // int key = rand() % (MAX_SIZE + 1);
    // int q = hash_search(T, key, op);
    // if (q != -1)
    //     printf("%d, op = %d\n", T[q].id, op.celule_accesate);
    // else
    //     printf("%d nu exista in tabela\n", key);
    printf("α = %.2lf\n", α);
}

void demo() {
    // DEFINE N 10 LA INCEPUT DOAR PENTRU DEMO
    Entry T[N];
    data op;
    int v[N];
    FillRandomArray(v, N, 1, MAX_SIZE, true, UNSORTED);
    for (int i = 0; i < N; i++)
        hash_insert(T, v[i]);
    for (int i = 0; i < N; i++)
        printf("%d: %d\n", i, T[i].id);
    for (int i = 0; i < N; i++)
        printf("%d: %d\n", v[i], hash_search(T, v[i], op));
}

int main() {
    srand(time(NULL));
    clock_t begin = clock();
    // demo();
    perf();
    clock_t end = clock();
    printf("%lf\n", (double)(end - begin) / CLOCKS_PER_SEC);
    return 0;
}
