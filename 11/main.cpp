#include <iostream>
#include <time.h>
#include <set>
#include "../Profiler.h"

#define MAX_SIZE 10000
#define STEP_SIZE 100
#define NR_TESTS 5


enum {
    COLOR_WHITE = 0,
    COLOR_GRAY,
    COLOR_BLACK
};

typedef struct {
    int row;
    int col;
} Point;


typedef struct _NodeT {
    int key;
    int rank;
    struct _NodeT *parent;
} NodeT;


typedef struct _Node {
    int key;

    int adjSize;
    struct _Node **adj;

    int color;
    int d;
    int f;
    struct _Node *parent;
} Node;

typedef struct {
    int nrNodes;
    Node **v;
} Graph;

void dfs(Graph *graph, Node *s, Operation *op = NULL);

NodeT *make_set(int key) {
    NodeT *x = (NodeT*)malloc(sizeof(NodeT));
    x->key = key;
    x->parent = x;
    x->rank = 0;
    return x;
}

NodeT *find_set(NodeT *x) {
    if (x != x->parent)
        x->parent = find_set(x->parent);
    return x->parent;
}

void link(NodeT **x, NodeT * (*y)) {
    if ((*x)->rank > (*y)->rank)
        (*y)->parent = (*x);

    else {
        (*x)->parent = (*y);
        if ((*x)->rank == (*y)->rank)
            (*y)->rank++;
    }
}

void unionn(NodeT **x, NodeT **y) {
    NodeT *set1 = find_set(*x);
    NodeT *set2 = find_set(*y);
    link(&set1, &set2);
}

void free_graph(Graph *graph) {
    if (graph->v != NULL) {
        for (int i = 0; i < graph->nrNodes; ++i) {
            if (graph->v[i] != NULL) {
                if (graph->v[i]->adj != NULL) {
                    free(graph->v[i]->adj);
                    graph->v[i]->adj = NULL;
                }
                graph->v[i]->adjSize = 0;
                free(graph->v[i]);
                graph->v[i] = NULL;
            }
        }
        free(graph->v);
        graph->v = NULL;
    }
    graph->nrNodes = 0;
}

void fa_graf_conex(Graph *graph, int n_edges) {
    Point *edges = (Point*)malloc(n_edges * sizeof(Point));
    bool a[graph->nrNodes][graph->nrNodes] = {false};
    for (int i = 0; i < graph->nrNodes; i++) {
        edges[i].row = i;
        edges[i].col = rand() % graph->nrNodes;
        while (edges[i].row == edges[i].col && a[edges[i].row][edges[i].col] == true)
            edges[i].col = rand() % graph->nrNodes;
        a[edges[i].row][edges[i].col] = true;
        // a[edges[i].col][edges[i].row] = true;
    }
    int k;

    NodeT *s[graph->nrNodes];
    for (int i = 0; i < graph->nrNodes; i++)
        s[i] = make_set(i);
    for (int i = 0; i < graph->nrNodes; i++)
        if (find_set(s[edges[i].row]) != find_set(s[edges[i].col]))
            unionn(&s[edges[i].row], &s[edges[i].col]);

    std::set<int>S;
    for (int i = 0; i < graph->nrNodes; i++) {
        S.insert(find_set(s[i])->key);
    }
    int connectedComponents = S.size();
    for (int i = graph->nrNodes; i < graph->nrNodes + connectedComponents; i++) {
        int u = *S.begin();
        S.erase(u);
        int v = *S.begin();
        unionn(&s[u], &s[v]);
        a[u][v] = a[v][u] = true;
        S.clear();
        for (int i = 0; i < graph->nrNodes; i++)
            S.insert(find_set(s[i])->key);
    }

    for (int i = graph->nrNodes + connectedComponents; i < n_edges; i++) {
        edges[i].row = rand() % graph->nrNodes;
        edges[i].col = rand() % graph->nrNodes;
        while (edges[i].row == edges[i].col && a[edges[i].row][edges[i].col] == true)
            edges[i].col = rand() % graph->nrNodes;
        a[edges[i].row][edges[i].col] = true;
        // a[edges[i].col][edges[i].row] = true;
    }
    for (int i = 0; i < graph->nrNodes; i++) {
        k = 0;
        // graph->v[i]->key = i;
        for (int j = 0; j < graph->nrNodes; j++)
            if (a[i][j])
                k++;
        graph->v[i]->adjSize = k;
        graph->v[i]->adj = (Node**)malloc(k * sizeof(Node*));
    }
    for (int i = 0; i < graph->nrNodes; i++) {
        k = 0;
        for (int j = 0; j < graph->nrNodes; j++)
            if (a[i][j])
                graph->v[i]->adj[k++] = graph->v[j];
    }
    if (edges != NULL)
        free(edges), edges = NULL;
}

void dfs(Graph *graph, Node *s, Operation *op) {
    static int time = 0;
    if (op != NULL)
        op->count(2);
    s->d = ++time;
    std::cout << s->key << ' ';
    s->color = COLOR_GRAY;
    for (int i = 0; i < s->adjSize; i++) {
        if (op != NULL)
            op->count();
        if (s->adj[i]->color == COLOR_WHITE) {
            if (op != NULL)
                op->count();
            s->adj[i]->parent = s;
            dfs(graph, s->adj[i], op);
        }
    }
    if (op != NULL)
        op->count(2);
    s->f = ++time;
    s->color = COLOR_BLACK;
}

void performance() {
    int n, i;
    Profiler p("dfs");

    // vary the number of edges
    for (n = 1000; n <= 4500; n += 100) {
        Operation op = p.createOperation("dfs-edges", n);
        Graph graph;
        graph.nrNodes = 100;
        //initialize the nodes of the graph
        graph.v = (Node**)malloc(graph.nrNodes * sizeof(Node*));
        for (i = 0; i < graph.nrNodes; ++i) {
            graph.v[i] = (Node*)malloc(sizeof(Node));
            memset(graph.v[i], 0, sizeof(Node));
        }
        // TODO: generate n random edges
        // make sure the generated graph is connected
        fa_graf_conex(&graph, n);
        dfs(&graph, graph.v[0], &op);
        free_graph(&graph);
    }

    // vary the number of vertices
    for (n = 100; n <= 200; n += 10) {
        Operation op = p.createOperation("dfs-vertices", n);
        Graph graph;
        graph.nrNodes = n;
        //initialize the nodes of the graph
        graph.v = (Node**)malloc(graph.nrNodes * sizeof(Node*));
        for (i = 0; i < graph.nrNodes; ++i) {
            graph.v[i] = (Node*)malloc(sizeof(Node));
            memset(graph.v[i], 0, sizeof(Node));
        }
        // TODO: generate 4500 random edges
        // make sure the generated graph is connected
        fa_graf_conex(&graph, 4500);
        dfs(&graph, graph.v[0], &op);
        free_graph(&graph);
    }

    p.showReport();
}


void demo() {
    Graph graph;
    graph.nrNodes = 10;
    graph.v = (Node**)malloc(graph.nrNodes * sizeof(Node*));
    for (int i = 0; i < graph.nrNodes; ++i) {
        graph.v[i] = (Node*)malloc(sizeof(Node));
        memset(graph.v[i], 0, sizeof(Node));
        graph.v[i]->key = i;
    }
    fa_graf_conex(&graph, 10);
    for (int i = 0; i < graph.nrNodes; i++) {
        std::cout << i << ": ";
        for (int j = 0; j < graph.v[i]->adjSize; j++)
            std::cout << graph.v[i]->adj[j]->key << ' ';
        std::cout << '\n';
    }
    for (int i = 0; i < graph.nrNodes; i++)
        if (graph.v[i]->color == COLOR_WHITE) {
            dfs(&graph, graph.v[i]);
            std::cout << '\n';
        }
    free_graph(&graph);
}

int main() {
    srand(time(nullptr));
    clock_t begin = clock();
    demo();
    // performance();
    clock_t end = clock();
    printf("\n%lf\n", (double)(end - begin) / CLOCKS_PER_SEC);
    return 0;
}