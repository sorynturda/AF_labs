#include <iostream>
#include <stdio.h>
#include <string.h>

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

void inorder(NodeT *root) {
    if (root == nullptr)
        return;
    inorder(root->left);
    std::cout << root->key << ' ';
    inorder(root->right);
}

NodeT *find_min(NodeT *x) {
    while (x->left)
        x = x->left;
    return x;
}

NodeT *find(NodeT *&x, int key) {
    if (x == NULL || x->key == key)
        return x;
    if (x->key > key)
        return find(x->left, key);
    return find(x->right, key);
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

void transplant(NodeT *&root, NodeT *&u, NodeT *&v) {
    if (u->p == nullptr) 
        root = v;
    else {
        if (u == u->p->left)
            u->p->left = v;
        else
            u->p->right = v;
    }
    if (v)
        v->p = u->p;
}

void update(NodeT *&x) {
    if (!x)
        return;
    update(x->p);
    x->size--;
}

void os_delete(NodeT *&root, NodeT *&x) {
    if (!x->left) {
        update(x);
        transplant(root, x, x->right);
    }
    else {
        if (!x->right) {
            update(x);
            transplant(root, x, x->left);
        }
        else {
            NodeT *y = find_min(x->right);
            update(y);
            if (y != x->right) {
                transplant(root, y, y->right);
                y->right = x->right;
                y->right->p = y;
            }
            transplant(root, x, y);
            y->left = x->left;
            y->left->p = y;
        }
    }
    root->size = root->left->size + root->right->size + 1;
}

void demo() {
    NodeT *root = nullptr;
    int n = 11;
    root = build_tree(root, 1, n);
    pp(root, 0);
    NodeT *x =  os_select(root, 3);
    os_delete(root, x);
    pp(root, 0);
    // os_delete(root, x);
    // pp(root, 0);
    // for (int i = 0; i < n; i++)
    //     if (os_select(root, i + 1)->p)
    //         std::cout << i + 1 << ": " << os_select(root, 1 + i)->p->key << '\n';
}

int main() {
    demo();
    return 0;
}