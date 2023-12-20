#include <stdlib.h>
#include <string.h>
#include <queue>
#include <set>
#include "bfs.h"

int get_neighbors(const Grid *grid, Point p, Point neighb[])
{
    // TODO: fill the array neighb with the neighbors of the point p and return the number of neighbors
    // the point p will have at most 4 neighbors (up, down, left, right)
    // avoid the neighbors that are outside the grid limits or fall into a wall
    // note: the size of the array neighb is guaranteed to be at least 4
    const int di[] = { -1, 1, 0, 0};
    const int dj[] = {0, 0, -1, 1};
    int count = 0;
    for (int k = 0; k < 4; k++)
    {
        int x = di[k] + p.row;
        int y = dj[k] + p.col;
        if (0 <= x && x < grid->rows && 0 <= y && y < grid->cols && grid->mat[x][y] == 0)
        {
            neighb[count].row = x;
            neighb[count].col = y;
            count++;
        }
    }
    return count;
}

void grid_to_graph(const Grid *grid, Graph *graph)
{
    //we need to keep the nodes in a matrix, so we can easily refer to a position in the grid
    Node *nodes[MAX_ROWS][MAX_COLS];
    int i, j, k;
    Point neighb[4];

    //compute how many nodes we have and allocate each node
    graph->nrNodes = 0;
    for (i = 0; i < grid->rows; ++i) {
        for (j = 0; j < grid->cols; ++j) {
            if (grid->mat[i][j] == 0) {
                nodes[i][j] = (Node*)malloc(sizeof(Node));
                memset(nodes[i][j], 0, sizeof(Node)); //initialize all fields with 0/NULL
                nodes[i][j]->position.row = i;
                nodes[i][j]->position.col = j;
                ++graph->nrNodes;
            } else {
                nodes[i][j] = NULL;
            }
        }
    }
    graph->v = (Node**)malloc(graph->nrNodes * sizeof(Node*));
    k = 0;
    for (i = 0; i < grid->rows; ++i) {
        for (j = 0; j < grid->cols; ++j) {
            if (nodes[i][j] != NULL) {
                graph->v[k++] = nodes[i][j];
            }
        }
    }

    //compute the adjacency list for each node
    for (i = 0; i < graph->nrNodes; ++i) {
        graph->v[i]->adjSize = get_neighbors(grid, graph->v[i]->position, neighb);
        if (graph->v[i]->adjSize != 0) {
            graph->v[i]->adj = (Node**)malloc(graph->v[i]->adjSize * sizeof(Node*));
            k = 0;
            for (j = 0; j < graph->v[i]->adjSize; ++j) {
                if ( neighb[j].row >= 0 && neighb[j].row < grid->rows &&
                        neighb[j].col >= 0 && neighb[j].col < grid->cols &&
                        grid->mat[neighb[j].row][neighb[j].col] == 0) {
                    graph->v[i]->adj[k++] = nodes[neighb[j].row][neighb[j].col];
                }
            }
            if (k < graph->v[i]->adjSize) {
                //get_neighbors returned some invalid neighbors
                graph->v[i]->adjSize = k;
                graph->v[i]->adj = (Node**)realloc(graph->v[i]->adj, k * sizeof(Node*));
            }
        }
    }
}

void free_graph(Graph *graph)
{
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

void bfs(Graph *graph, Node *s, Operation *op)
{
    // TOOD: implement the BFS algorithm on the graph, starting from the node s
    // at the end of the algorithm, every node reachable from s should have the color BLACK
    // for all the visited nodes, the minimum distance from s (dist) and the parent in the BFS tree should be set
    // for counting the number of operations, the optional op parameter is received
    // since op can be NULL (when we are calling the bfs for display purposes), you should check it before counting:
    // if(op != NULL) op->count();
    for (int i = 0; i < graph->nrNodes; i++)
        if (graph->v[i] != s)
        {
            graph->v[i]->color = COLOR_WHITE;
            graph->v[i]->dist = 1e9;
            graph->v[i]->parent = NULL;
        }
    if (op != NULL)
        op->count(4);
    s->color = COLOR_WHITE;
    s->dist = 0;
    s->parent = NULL;
    std::queue<Node*> Q;
    Q.push(s);
    while (!Q.empty())
    {
        if (op != NULL)
            op->count();
        Node *u = Q.front();
        Q.pop();
        for (int i = 0; i < u->adjSize; i++) {
            if (op != NULL)
                op->count();
            if (u->adj[i]->color == COLOR_WHITE)
            {
                if (op != NULL)
                    op->count(3);
                u->adj[i]->color = COLOR_GRAY;
                u->adj[i]->dist = u->dist + 1;
                u->adj[i]->parent = u;
                Q.push(u->adj[i]);
            }
        }
        if (op != NULL)
            op->count();
        u->color = COLOR_BLACK;
    }
}

void print_bfs_tree(Graph *graph)
{
    //first, we will represent the BFS tree as a parent array
    int n = 0; //the number of nodes
    int *p = NULL; //the parent array
    Point *repr = NULL; //the representation for each element in p

    //some of the nodes in graph->v may not have been reached by BFS
    //p and repr will contain only the reachable nodes
    int *transf = (int*)malloc(graph->nrNodes * sizeof(int));
    for (int i = 0; i < graph->nrNodes; ++i) {
        if (graph->v[i]->color == COLOR_BLACK) {
            transf[i] = n;
            ++n;
        } else {
            transf[i] = -1;
        }
    }
    if (n == 0) {
        //no BFS tree
        free(transf);
        return;
    }

    int err = 0;
    p = (int*)malloc(n * sizeof(int));
    repr = (Point*)malloc(n * sizeof(Node));
    for (int i = 0; i < graph->nrNodes && !err; ++i) {
        if (graph->v[i]->color == COLOR_BLACK) {
            if (transf[i] < 0 || transf[i] >= n) {
                err = 1;
            } else {
                repr[transf[i]] = graph->v[i]->position;
                if (graph->v[i]->parent == NULL) {
                    p[transf[i]] = -1;
                } else {
                    err = 1;
                    for (int j = 0; j < graph->nrNodes; ++j) {
                        if (graph->v[i]->parent == graph->v[j]) {
                            if (transf[j] >= 0 && transf[j] < n) {
                                p[transf[i]] = transf[j];
                                err = 0;
                            }
                            break;
                        }
                    }
                }
            }
        }
    }
    free(transf);
    transf = NULL;

    if (!err) {
        // TODO: pretty print the BFS tree
        // the parrent array is p (p[k] is the parent for node k or -1 if k is the root)
        // when printing the node k, print repr[k] (it contains the row and column for that point)
        // you can adapt the code for transforming and printing multi-way trees from the previous labs
        preety_print(repr, p, n, -1, 0);
    }

    if (p != NULL) {
        free(p);
        p = NULL;
    }
    if (repr != NULL) {
        free(repr);
        repr = NULL;
    }
}

void preety_print(Point repr[], int p[], int n, int parinte, int spatii) {
    char s[spatii + 1];
    memset(s, ' ', spatii);
    s[spatii] = '\0';
    for (int i = 0; i < n; i++) {
        if (p[i] == parinte) {
            printf("%s(%d, %d)\n", s, repr[i].row, repr[i].col);
            preety_print(repr, p, n, i, spatii + 8);
        }
    }
}


int shortest_path(Graph *graph, Node *start, Node *end, Node *path[])
{
    // TODO: compute the shortest path between the nodes start and end in the given graph
    // the nodes from the path, should be filled, in order, in the array path
    // the number of nodes filled in the path array should be returned
    // if end is not reachable from start, return -1
    // note: the size of the array path is guaranteed to be at least 1000
    bfs(graph, start);
    if (end->color == COLOR_WHITE)
        return -1;
    int n = 0;
    Node *e = end;
    while (e != start) {
        path[n++] = e;
        e = e->parent;
    }
    for (int i = 0; i < n / 2; i++)
        std::swap(path[i], path[n - i - 1]);
    return n;
}

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

void fa_graf_conex(Graph *graph, int n_edges) {
    srand(time(NULL));
    Point *edges = (Point*)malloc(n_edges * sizeof(Point));
    bool a[graph->nrNodes][graph->nrNodes] = {false};
    for (int i = 0; i < graph->nrNodes; i++) {
        edges[i].row = i;
        edges[i].col = rand() % graph->nrNodes;
        while (edges[i].row == edges[i].col && a[edges[i].col][edges[i].row] == true)
            edges[i].col = rand() % graph->nrNodes;
        a[edges[i].row][edges[i].col] = true;
        a[edges[i].col][edges[i].row] = true;
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
        while (edges[i].row == edges[i].col && a[edges[i].col][edges[i].row] == true)
            edges[i].col = rand() % graph->nrNodes;
        a[edges[i].row][edges[i].col] = true;
        a[edges[i].col][edges[i].row] = true;
    }
    for (int i = 0; i < graph->nrNodes; i++) {
        k = 0;
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

void performance()
{
    int n, i;
    Profiler p("bfs");

    // vary the number of edges
    for (n = 1000; n <= 4500; n += 100) {
        Operation op = p.createOperation("bfs-edges", n);
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
        bfs(&graph, graph.v[0], &op);
        free_graph(&graph);
    }

    // vary the number of vertices
    for (n = 100; n <= 200; n += 10) {
        Operation op = p.createOperation("bfs-vertices", n);
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
        bfs(&graph, graph.v[0], &op);
        free_graph(&graph);
    }

    p.showReport();
}
