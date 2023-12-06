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

void demo() {
    int n = 10;
    NodeT *set1 = make_set(1);
    NodeT *set2 = make_set(2);
    NodeT *set3 = make_set(3);
    unionn(set1, set3);
    unionn(set1, set2);
    std::cout << find_set(set1)->key << ": " << find_set(set1)->rank << '\n';
    std::cout << find_set(set2)->key << ": " << find_set(set2)->rank << '\n';
    std::cout << find_set(set3)->key << ": " << find_set(set3)->rank << '\n';
}

int main() {
    clock_t begin = clock();
    demo();
    clock_t end = clock();
    printf("%lf\n", (double)(end - begin) / CLOCKS_PER_SEC);
    return 0;
}