#include <iostream>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "../Profiler.h"

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

edge *kruskal(edge edges[], int n) {
    int count = 0;
    NodeT *set[n / 4];
    edge *ans = (edge*)malloc((n / 4 - 1) * sizeof(edge));
    for (int i = 0; i < n / 4; i++)
        set[i] = make_set(i);
    quicksort(edges, 0, n - 1);
    for (int i = 0; i < n; i++) {
        if (find_set(set[edges[i].u]) != find_set(set[edges[i].v])) {
            ans[count++] = edges[i];
            unionn(set[edges[i].u], set[edges[i].v]);
        }
    }
    return ans;
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
    int n = 4;
    edge edges[] = {
        {0, 1, 5}, {1, 2, 3}, {2, 3, 7}, {3, 0, 4},
        {0, 2, 3}, {0, 3, 1}, {1, 3, 2}, {1, 3, 4},
        {0, 1, 8}, {1, 2, 7}, {0, 2, 1}, {0, 3, 5},
        {0, 2, 9}, {2, 3, 5}, {1, 3, 4}, {1, 3, 7}
    };
    // quicksort(edges,0,15);
    // for (int i = 0; i < 4 * n; i++)
    //     printf("%d %d %d\n", edges[i].u, edges[i].v, edges[i].w);
    // puts("");
    edge *ans = kruskal(edges, 4 * n);
    for (int i = 0; i < n - 1; i++)
        printf("%d - %d: %d\n", ans[i].u, ans[i].v, ans[i].w);
}

int main() {
    clock_t begin = clock();
    // demo();
    demo_kruskal();
    clock_t end = clock();
    printf("%lf\n", (double)(end - begin) / CLOCKS_PER_SEC);
    return 0;
}