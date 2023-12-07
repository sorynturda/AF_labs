#include <iostream>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <set>
#include "../Profiler.h"

#define MAX_SIZE 10000
#define STEP_SIZE 100
#define NR_TESTS 5

struct NodeT {
    int key;
    int rank;
    NodeT *parent;
};

struct edge {
    int u;
    int v;
    int w;
};

int partition(edge a[], int left, int right) {
    edge x = a[right];
    // ops.count();
    int i = left - 1;
    for (int j = left; j <= right - 1; j++) {
        // ops.count();
        if (a[j].w <= x.w) {
            i++;
            // ops.count(3);
            std::swap(a[i], a[j]);
        }
    }
    // ops.count();
    std::swap(a[i + 1], a[right]);
    return i + 1;
}

void quicksort(edge a[], int left, int right) {
    if (left >= right)
        return;
    int q = partition(a, left, right);
    quicksort(a, left, q - 1);
    quicksort(a, q + 1, right);
}

NodeT *make_set(int key) {
    NodeT *x = new NodeT;
    x->key = key;
    x->parent = x;
    x->rank = 0;
    return x;
}

NodeT *find_set(NodeT *&x) {
    if (x != x->parent)
        x->parent = find_set(x->parent);
    return x->parent;
}

void link(NodeT *&x, NodeT *&y) {
    if (x->rank > y->rank)
        y->parent = x;
    else {
        x->parent = y;
        if (x->rank == y->rank)
            y->rank++;
    }
}

void unionn(NodeT *&x, NodeT *&y) {
    NodeT *set1 = find_set(x);
    NodeT *set2 = find_set(y);
    link(set1, set2);
}

edge *kruskal(edge edges[], int n, int &m) {
    int count = 0;
    NodeT *set[n];
    edge *res = (edge*)malloc((n - 1) * sizeof(edge));
    for (int i = 0; i < n; i++)
        set[i] = make_set(i);
    quicksort(edges, 0, m - 1);
    for (int i = 0; i < m; i++) {
        if (find_set(set[edges[i].u]) != find_set(set[edges[i].v])) {
            res[count++] = edges[i];
            unionn(set[edges[i].u], set[edges[i].v]);
        }
    }
    // if (m == n) {
    //     std::set<int>S;
    //     for (int i = 0; i < n; i++)
    //         S.insert(find_set(set[i])->key);
    //     for (int i = 0; i < n; i++)
    //         std::cout << find_set(set[i])->key << ' ';
    //     m = S.size();
    // }
    return res;
}

edge get_edge(int u, int n) {
    edge res;
    res.u = u;
    int v = rand() % n;
    while (v == u)
        v = rand() % n;
    res.v = v;
    res.w = rand() % MAX_SIZE;
    return res;
}

void demo() {
    int n = 10;
    NodeT *set[n];
    for (int i = 0; i < n; i++)
        set[i] = make_set(i);
    for (int i = 0; i < n; i++)
        std::cout << find_set(set[i])->key << ' ';
    puts("");
    unionn(set[0], set[1]);
    unionn(set[3], set[7]);
    unionn(set[5], set[6]);
    unionn(set[5], set[9]);
    for (int i = 0; i < n; i++)
        std::cout << find_set(set[i])->key << ' ';
    puts("");
}

void demo_kruskal() {
    int n = 4, m = n * 4;
    edge edges[] = {
        {0, 1, 5}, {1, 2, 3}, {2, 3, 7}, {3, 0, 4},
        {0, 2, 3}, {0, 3, 1}, {1, 3, 2}, {1, 3, 4},
        {0, 1, 8}, {1, 2, 7}, {0, 2, 1}, {0, 3, 5},
        {0, 2, 9}, {2, 3, 5}, {1, 3, 4}, {1, 3, 7}
    };
    // quicksort(edges,0,15);
    // for (int i = 0; i < m; i++)
    //     printf("%d %d %d\n", edges[i].u, edges[i].v, edges[i].w);
    // puts("");
    edge *res = kruskal(edges, n, m);
    for (int i = 0; i < n - 1; i++)
        printf("%d - %d: %d\n", res[i].u, res[i].v, res[i].w);
}

void perf() {
    edge edges[100];
    // for (int n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE) {
    //     for (int u = 0; u < n; u++) {
    //         edges[i] = get_edge(u, n);
    //     }
    // }
    int m = 100;
    for (int u = 0; u < 100; u++)
        edges[u] = get_edge(u, 100);
    edge *res = kruskal(edges, 100, m);
    std::cout << '\n' << m << '\n';
}

int main() {
    srand(time(nullptr));
    clock_t begin = clock();
    // demo();
    // demo_kruskal();
    perf();
    clock_t end = clock();
    printf("%lf\n", (double)(end - begin) / CLOCKS_PER_SEC);
    return 0;
}