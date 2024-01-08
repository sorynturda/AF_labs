#include <iostream>
#include <time.h>
#include <set>
#include <vector>
#include <stack>
#include "/home/sorin/Documents/facultate/AF_labs/Profiler.h"

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

    bool onStack;
    int index;
    int lowLink;
    int comp;

    struct _Node *parent;
} Node;

typedef struct {
    int nrNodes;
    Node **v;
} Graph;

void strong_connect(Graph *graph, Node *s);

void dfs(Graph *graph, Node *s, Operation *op = NULL);

std::stack<Node *> stiva;

int INDEX = 0, nrComponents = 0;
bool cycle = false;

NodeT *make_set(int key) {
    NodeT *x = (NodeT *) malloc(sizeof(NodeT));
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

void link(NodeT **x, NodeT *(*y)) {
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

void fa_graf(Graph *graph, int n_edges) {
    bool a[graph->nrNodes][graph->nrNodes];
    for (int i = 0; i < graph->nrNodes; i++)for (int j = 0; j < graph->nrNodes; j++)a[i][j] = 0;
    for (int i = 0; i < n_edges; i++) {
        int u = rand() % graph->nrNodes;
        int v = rand() % graph->nrNodes;
        while (u == v || a[u][v])
            v = rand() % graph->nrNodes;
        a[u][v] = true;
        // a[v][u] = true;
    }
    int k;
    for (int i = 0; i < graph->nrNodes; i++) {
        k = 0;
        // graph->v[i]->key = i;
        for (int j = 0; j < graph->nrNodes; j++)
            if (a[i][j])
                k++;
        graph->v[i]->adjSize = k;
        graph->v[i]->adj = (Node **) malloc(k * sizeof(Node *));
    }
    for (int i = 0; i < graph->nrNodes; i++) {
        k = 0;
        for (int j = 0; j < graph->nrNodes; j++)
            if (a[i][j])
                graph->v[i]->adj[k++] = graph->v[j];
    }
}

void dfs(Graph *graph, Node *s, Operation *op) {
    static int time = 0;
    if (op != NULL)
        op->count(2);
    s->d = ++time;
    // std::cout << s->key << ' ';
    s->color = COLOR_GRAY;
    for (int i = 0; i < s->adjSize; i++) {
        if (op != NULL)
            op->count();
        Node *v = s->adj[i];
        if (v->parent && v->d && v->f) {
            cycle = true;
//            std::cout<<s->key<<' '<<s->adj[i]->key<<'\n';
        }
        if (v->color == COLOR_WHITE) {
            if (op != NULL)
                op->count();
            v->parent = s;
            dfs(graph, s->adj[i], op);
        }
    }
    if (op != NULL)
        op->count(2);
    s->f = ++time;
    stiva.push(s);
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
        graph.v = (Node **) malloc(graph.nrNodes * sizeof(Node *));
        for (i = 0; i < graph.nrNodes; ++i) {
            graph.v[i] = (Node *) malloc(sizeof(Node));
            memset(graph.v[i], 0, sizeof(Node));
        }
        // TODO: generate n random edges
        // make sure the generated graph is connected
        fa_graf(&graph, n);
        dfs(&graph, graph.v[0], &op);
        free_graph(&graph);
    }

    // vary the number of vertices
    for (n = 100; n <= 200; n += 10) {
        Operation op = p.createOperation("dfs-vertices", n);
        Graph graph;
        graph.nrNodes = n;
        //initialize the nodes of the graph
        graph.v = (Node **) malloc(graph.nrNodes * sizeof(Node *));
        for (i = 0; i < graph.nrNodes; ++i) {
            graph.v[i] = (Node *) malloc(sizeof(Node));
            memset(graph.v[i], 0, sizeof(Node));
        }
        // TODO: generate 4500 random edges
        // make sure the generated graph is connected
        fa_graf(&graph, 4500);
        dfs(&graph, graph.v[0], &op);
        free_graph(&graph);
    }

    p.showReport();
}

void tarjan(Graph *graph) {
    for (int i = 0; i < graph->nrNodes; i++) {
        graph->v[i]->index = graph->v[i]->lowLink = -1;
        graph->v[i]->onStack = false;
        graph->v[i]->comp = 0;
    }
    for (int i = 0; i < graph->nrNodes; i++)
        if (graph->v[i]->index == -1)
            strong_connect(graph, graph->v[i]);
}

void strong_connect(Graph *graph, Node *s) {
//     std::cout<<"ADS\n";
    s->index = s->lowLink = INDEX;
    INDEX++;
    stiva.push(s);
    s->onStack = true;
    for (int i = 0; i < s->adjSize; i++)
        if (s->adj[i]->index == -1) {
            strong_connect(graph, s->adj[i]);
            s->lowLink = std::min(s->adj[i]->lowLink, s->lowLink);
        } else if (s->adj[i]->onStack)
            s->lowLink = std::min(s->lowLink, s->adj[i]->index);
    if (s->lowLink == s->index) {
        nrComponents++;
        Node *u;
        do {
            if(stiva.empty())
                break;
            u = stiva.top();
            stiva.pop();
            u->onStack = false;
            u->comp = nrComponents;
        } while (s == u);
    }
}

void ddemo() {
    Graph graph;
    graph.nrNodes = 5;
    graph.v = (Node **) malloc(graph.nrNodes * sizeof(Node *));
    for (int i = 0; i < graph.nrNodes; ++i) {
        graph.v[i] = (Node *) malloc(sizeof(Node));
        memset(graph.v[i], 0, sizeof(Node));
        graph.v[i]->key = i;
        graph.v[i]->color = COLOR_WHITE;
        graph.v[i]->f = 0;
        graph.v[i]->d = 0;
        graph.v[i]->parent = NULL;
    }
    fa_graf(&graph, 6);
    for (int i = 0; i < graph.nrNodes; i++) {
        std::cout << i << ": ";
        for (int j = 0; j < graph.v[i]->adjSize; j++)
            std::cout << graph.v[i]->adj[j]->key << ' ';
        std::cout << '\n';
    }
    tarjan(&graph);
    std::cout << nrComponents << '\n';
    for (int i = 0; i < graph.nrNodes; i++)
        std::cout << i << ": " << graph.v[i]->lowLink << '\n';
    free_graph(&graph);
}

void demo() {
    Graph graph;
    graph.nrNodes = 10;
    graph.v = (Node **) malloc(graph.nrNodes * sizeof(Node *));
    for (int i = 0; i < graph.nrNodes; ++i) {
        graph.v[i] = (Node *) malloc(sizeof(Node));
        memset(graph.v[i], 0, sizeof(Node));
        graph.v[i]->key = i;
        graph.v[i]->color = COLOR_WHITE;
        graph.v[i]->f = 0;
        graph.v[i]->d = 0;
        graph.v[i]->parent = NULL;
    }
//    std::cout << "ASD";
    fa_graf(&graph, 3);
    for (int i = 0; i < graph.nrNodes; i++) {
        std::cout << i << ": ";
        for (int j = 0; j < graph.v[i]->adjSize; j++)
            std::cout << graph.v[i]->adj[j]->key << ' ';
        std::cout << '\n';
    }
    for (int i = 0; i < graph.nrNodes; i++)
        if (graph.v[i]->color == COLOR_WHITE) {
            dfs(&graph, graph.v[i]);
            // std::cout << '\n';
        }
    if (!cycle)
        while (!stiva.empty()) {
            std::cout << stiva.top()->key << ' ';
            stiva.pop();
        }
    else
        std::cout << "ciclu\n";
    free_graph(&graph);
}

int main() {
    srand(time(nullptr));
    clock_t begin = clock();
//    demo();
    ddemo();
//     performance();
    clock_t end = clock();
    printf("\n%lf\n", (double) (end - begin) / CLOCKS_PER_SEC);
    return 0;
}