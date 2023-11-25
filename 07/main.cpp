#include <iostream>
#include <stdio.h>
#include <string.h>

struct NodeT {
    int key;
    int size;
    NodeT *left;
    NodeT *right;
};

NodeT *create(int key) {
    NodeT *p = new NodeT;
    p->left = nullptr;
    p->right = nullptr;
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

NodeT *find(NodeT *x, int key) {
    if (x == NULL || x->key == key)
        return x;
    if (x->key > key)
        return find(x->left, key);
    return find(x->right, key);
}

NodeT *succesor(NodeT *x, NodeT *root) {
    if (x->right)
        return find_min(x->right);
    NodeT *p = root, *q = nullptr;
    while (p != x) {
        if (x->key > p->key)
            p = p->right;
        else {
            q = p;
            p = p->left;
        }
    }
    if (q == nullptr)
        return p;
    return q;
}

NodeT *function(int left, int right) {
    if (left > right)
        return nullptr;
    int mid = left + (right - left) / 2;
    NodeT *x = create(mid);
    x->left = function(left, mid - 1);
    x->right = function(mid + 1, right);
    x->size = right - left + 1;
    return x;

}

NodeT *build_tree(int n) {
    NodeT *root = function(1, n);
    return root;
}

NodeT *os_select(NodeT *x, int i) {
    int r = x->left ? x->left->size + 1 : 1;
    if (i == r)
        return x;
    if (i < r)
        return os_select(x->left, i);
    return os_select(x->right, i - r);
}

void demo() {
    NodeT *root = nullptr;
    int n = 11;
    root = build_tree(n);
    pp(root, 0);
    std::cout << succesor(find(root, 4), root)->key << '\n';
    // for (int i = 0; i < n; i++)
    //     std::cout << os_select(root, 1 + i)->key << '\n';

}

int main() {
    demo();
    return 0;
}