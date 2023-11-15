#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unordered_set>
#include <limits.h>
#include <time.h>
#include <unistd.h>
#include "../Profiler.h"

#define MAX_SIZE  40000
#define N 10007 //aici pentru demo
#define M 3000
#define NR_TESTS 5
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
    int efort_maxim_gasite = 0;
    int efort_maxim_negasite = 0;
    double efort_mediu_gasite = 0;
    double efort_mediu_negasite = 0;
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

int hash_search(Entry T[], int key, int &accesari) {
    int i = 0, q;
    do {
        q = h(key, i);
        accesari++;
        if (T[q].free == false && T[q].id == key)
            return q;
        else
            i++;
    } while (T[q].used == true && i < N);
    return -1;
}

void hash_delete(Entry T[], int key, int &acc) {
    int q = hash_search(T, key, acc);
    if (q == -1)
        return;
    T[q].free = true;
}

std::unordered_set<int> ia_elemente(Entry T[], int n) {
    int k = 0;
    std::unordered_set<int> res;
    while (k <= n) {
        int pos = rand() % N;
        while (T[pos].id == 0)
            pos = rand() % N;
        if (res.find(T[pos].id) == res.end()) {
            res.insert(T[pos].id);
            k++;
        }
    }
    return res;
}

void perf() {
    FILE *f = fopen("rez.csv", "w");
    fprintf(f, "Factor de umplere,Efort mediu gasite,\
        Efort maxim gasite,Efort mediu negasite,\
        Efort maxim negasite\n");
    int array[MAX_SIZE];
    double val[] = {0.8, 0.85, 0.9, 0.95, 0.99};
    int nr_val = sizeof(val) / sizeof(val[0]);
    data pt_csv[nr_val];
    for (int k = 0; k < nr_val; k++) {
        Entry T[N];
        int n = 0;
        double α = 0;
        FillRandomArray(array, MAX_SIZE, 1, MAX_SIZE, true, UNSORTED);
        for (int i = 0; α < val[k] && i < MAX_SIZE; i++) {
            if (hash_insert(T, array[i]) != -1)
                n++;
            α = 1.0 * n / N;
        }
        std::unordered_set<int> s = ia_elemente(T, M / 2 - 1);
        // print(T);
        int tot = 0;
        for (auto it : s) {
            int accesari = 0;
            hash_search(T, it, accesari);
            pt_csv[k].efort_maxim_gasite = std::max(pt_csv[k].efort_maxim_gasite, accesari);
            tot += accesari;
        }
        pt_csv[k].efort_mediu_gasite = (double)tot / 1500;
        int v[M / 2 - 1];
        tot = 0;
        FillRandomArray(v, M / 2 - 1, MAX_SIZE + 1, MAX_SIZE * 2, true, UNSORTED);
        for (int jj = 0; jj < M / 2 - 1; jj++) {
            int accesari = 0;
            hash_search(T, v[jj], accesari);
            pt_csv[k].efort_maxim_negasite = std::max(pt_csv[k].efort_maxim_negasite, accesari);
            tot += accesari;
        }
        pt_csv[k].efort_mediu_negasite = (double) tot / 1500;
        // std::cout << s.size() << '\n';
        printf("α = %.2lf\n", α);
    }
    for (int i = 0; i < nr_val; i++) {
        fprintf(f, "%0.2lf,%lf,%d,%lf,%d\n", val[i], pt_csv[i].efort_mediu_gasite,
                pt_csv[i].efort_maxim_gasite, pt_csv[i].efort_mediu_negasite,
                pt_csv[i].efort_maxim_negasite);
    }
    // printf("gasite: %d\naccesate: %d\nmaxim %d\n", gasite, op.celule_accesate, cel);
    // int key = rand() % (MAX_SIZE + 1);
    // int q = hash_search(T, key, op);
    // if (q != -1)
    //     printf("%d, op = %d\n", T[q].id, op.celule_accesate);
    // else
    //     printf("%d nu exista in tabela\n", key);
    fclose(f);
}

void demo() {
    // DEFINE N 10 LA INCEPUT DOAR PENTRU DEMO
    Entry T[N];
    int acc = 0;
    int v[N];
    FillRandomArray(v, N, 1, MAX_SIZE, true, UNSORTED);
    for (int i = 0; i < N; i++)
        hash_insert(T, v[i]);
    for (int i = 0; i < N; i++)
        printf("%d: %d\n", i, T[i].id);
    for (int i = 0; i < N; i++)
        printf("%d: %d\n", v[i], hash_search(T, v[i], acc));
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
