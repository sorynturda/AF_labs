#include <iostream>
#include <stdio.h>
#include <string.h>
#include <time.h>

struct NodeT {
    int key;
    int size;
    NodeT *left;
    NodeT *right;
    NodeT *p;
};

NodeT *create(int key) {
    NodeT *p = new NodeT;
    p->left = nullptr;
    p->right = nullptr;
    p->p = nullptr;
    p->key = key;
    p->size = 0;
    return p;
}

void pp(NodeT *root, int indent) {
    if (root == nullptr)
        return;
    char s[indent + 1];
    memset(s, ' ', indent);
    s[indent] = '\0';
    printf("%s%d(%d)\n", s, root->key, root->size);
    pp(root->right, indent + 4);
    pp(root->left, indent + 4);
}

NodeT *find_min(NodeT *x) {
    while (x->left)
        x = x->left;
    return x;
}

NodeT *succesor(NodeT *&x) {
    if (x->right)
        return find_min(x->right);
    NodeT *y = x->p;
    while (y && x == y->right) {
        x = y;
        y = y->p;
    }
    return y;
}

void update(NodeT *&x) {
    if (x == nullptr)
        return;
    update(x->p);
    x->size--;
}

NodeT *build_tree(NodeT *&y, int left, int right) {
    if (left > right)
        return nullptr;
    int mid = left + (right - left) / 2;
    NodeT *x = create(mid);
    x->left = build_tree(x, left, mid - 1);
    x->right = build_tree(x, mid + 1, right);
    x->p = y;
    x->size = right - left + 1;
    return x;

}

NodeT *os_select(NodeT *&x, int i) {
    int r = x->left ? x->left->size + 1 : 1;
    if (i == r)
        return x;
    if (i < r)
        return os_select(x->left, i);
    return os_select(x->right, i - r);
}

void os_delete(NodeT *&root, NodeT *&z) {
    NodeT *y, *x;
    if (!z->left || !z->right)
        y = z;
    else
        y = succesor(z);
    if (y->left)
        x = y->left;
    else
        x = y->right;
    if (x)
        x->p = y->p;
    if (!y->p)
        root = x;
    else if (y == y->p->left)
        y->p->left = x;
    else
        y->p->right = x;
    if (y != z)
        z->key = y->key;
    update(y);
}

void demo() {
    NodeT *root = nullptr;
    int n = 11;
    pp(root, 0);
    root = build_tree(root, 1, n);
    pp(root,0);
    for (int i = 0; i < 3; i++) {
        int x = rand() % n + 1;
        NodeT *y;
        printf("x = %d\n", x);
        y = os_select(root, x);
        os_delete(root, y);
        pp(root, 0);
        n--;
    }
}

int main() {
    srand(time(nullptr));
    demo();
    return 0;
}