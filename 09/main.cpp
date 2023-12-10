/*
 * Sorin Turda, grupa 30222
 * Algoritmul lui kruskal are complexitate O(E * log E), E fiind numarul de muchii
 * find-set are complexitatea O(1)
 * make-set are complexitatea O(1)
 * toate operatiile pe multimi disjuncte au complexitatea O(m + n*lg (n)), m operatii si n multimi
 */
#include <iostream>
#include <time.h>
#include <set>
#include "../Profiler.h"

#define MAX_SIZE 10000
#define STEP_SIZE 100
#define NR_TESTS 5

Profiler p("kruskal");

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
    int i = left - 1;
    for (int j = left; j <= right - 1; j++) {
        if (a[j].w <= x.w) {
            i++;
            std::swap(a[i], a[j]);
        }
    }
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

NodeT *make_set(int key, Operation ops) {
    NodeT *x = new NodeT;
    ops.count(3);
    x->key = key;
    x->parent = x;
    x->rank = 0;
    return x;
}

NodeT *find_set(NodeT *&x, Operation ops) {
    if (x != x->parent) {
        ops.count();
        x->parent = find_set(x->parent);
    }
    return x->parent;
}

void link(NodeT *&x, NodeT *&y, Operation ops) {
    if (x->rank > y->rank) {
        ops.count();
        y->parent = x;
    }
    else {
        ops.count(2);
        x->parent = y;
        if (x->rank == y->rank) {
            ops.count();
            y->rank++;
        }
    }
}

void unionn(NodeT *&x, NodeT *&y, Operation ops) {
    NodeT *set1 = find_set(x);
    NodeT *set2 = find_set(y);
    link(set1, set2, ops);
}

edge *kruskal(edge edges[], int n, int m) {
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
    return res;
}

edge *kruskal(edge edges[], int n, int m, Operation ops) {
    int count = 0;
    NodeT *set[n];
    edge *res = (edge*)malloc((n - 1) * sizeof(edge));
    for (int i = 0; i < n; i++)
        set[i] = make_set(i, ops);
    quicksort(edges, 0, m - 1);
    for (int i = 0; i < m; i++) {
        if (find_set(set[edges[i].u], ops) != find_set(set[edges[i].v], ops)) {
            res[count++] = edges[i];
            unionn(set[edges[i].u], set[edges[i].v], ops);
        }
    }
    return res;
}

void fa_graf_conex(edge edges[], int n, int &m) {
    NodeT *s[n];
    for (int i = 0; i < n; i++)
        s[i] = make_set(i);
    for (int i = 0; i < m; i++) {
        if (find_set(s[edges[i].u]) != find_set(s[edges[i].v]))
            unionn(s[edges[i].u], s[edges[i].v]);
    }
    std::set<int>S;
    for (int i = 0; i < n; i++) {
        S.insert(find_set(s[i])->key);
    }
    m = m + S.size() - 1;
    for (int i = n; i < m; i++) {
        int u = *S.begin();
        S.erase(u);
        int v = *S.begin();
        unionn(s[u], s[v]);
        edges[i].u = u;
        edges[i].v = v;
        edges[i].w = rand() % MAX_SIZE + 1;
        S.clear();
        for (int i = 0; i < n; i++)
            S.insert(find_set(s[i])->key);
    }
}

edge get_edge(int u, int n) {
    edge res;
    res.u = u;
    int v = rand() % n;
    while (v == u)
        v = rand() % n;
    res.v = v;
    res.w = rand() % MAX_SIZE + 1;
    return res;
}

edge get_edge(int n) {
    edge res;
    res.u = rand() % n;
    res.v = rand() % n;
    while (res.u == res.v)
        res.v = rand() % n;
    res.w = rand() % MAX_SIZE + 1;
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
    edge *res = kruskal(edges, n, m);
    for (int i = 0; i < n - 1; i++)
        printf("%d - %d: %d\n", res[i].u, res[i].v, res[i].w);
}

void perf() {
    edge edges[MAX_SIZE * 4];
    for (int n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE) {
        for (int test = 0; test < NR_TESTS; test++) {
            for (int u = 0; u < n; u++) {
                edges[u] = get_edge(u, n);
            }
            int m = n;
            fa_graf_conex(edges, n, m);
            for (int i = m; i < n * 4; i++)
                edges[i] = get_edge(n);
            m = n * 4;
            Operation ops = p.createOperation("kruskal", n);
            kruskal(edges, n, m, ops);
        }
    }
    p.divideValues("kruskal", NR_TESTS);
    p.showReport();
}

int main() {
    srand(time(nullptr));
    clock_t begin = clock();
    demo();
    demo_kruskal();
    // perf();
    clock_t end = clock();
    printf("%lf\n", (double)(end - begin) / CLOCKS_PER_SEC);
    return 0;
}