// OrderBST.cpp
// CS212 Project 94 - Tailoring Order Workshop
#include "OrderBST.h"

OrderBST::OrderBST() : root(nullptr), count(0) {}

OrderBST::~OrderBST() { destroy(root); }

void OrderBST::destroy(Node* n) {
    if (n == nullptr) return;
    destroy(n->left);
    destroy(n->right);
    delete n;
}

bool OrderBST::insert(const Order& o) {
    Node* parent = nullptr;
    Node* cur = root;
    while (cur != nullptr) {
        if (o.id == cur->data.id) return false;   // duplicate ID -> reject
        parent = cur;
        cur = (o.id < cur->data.id) ? cur->left : cur->right;
    }
    Node* node = new Node(o);
    if (parent == nullptr) root = node;
    else if (o.id < parent->data.id) parent->left = node;
    else parent->right = node;
    ++count;
    return true;
}

OrderBST::Node* OrderBST::minNode(Node* n) {
    while (n != nullptr && n->left != nullptr) n = n->left;
    return n;
}

// Recursive delete. Handles: leaf, one child, two children (inorder successor),
// and the root-only tree (returns nullptr so root becomes empty).
OrderBST::Node* OrderBST::removeNode(Node* n, int id, bool& removed) {
    if (n == nullptr) return nullptr;
    if (id < n->data.id) {
        n->left = removeNode(n->left, id, removed);
    } else if (id > n->data.id) {
        n->right = removeNode(n->right, id, removed);
    } else {
        removed = true;
        if (n->left == nullptr) {
            Node* r = n->right;
            delete n;
            return r;
        }
        if (n->right == nullptr) {
            Node* l = n->left;
            delete n;
            return l;
        }
        Node* succ = minNode(n->right);          // two children
        n->data = succ->data;
        bool dummy = false;
        n->right = removeNode(n->right, succ->data.id, dummy);
    }
    return n;
}

bool OrderBST::remove(int id) {
    bool removed = false;
    root = removeNode(root, id, removed);
    if (removed) --count;
    return removed;
}

Order* OrderBST::search(int id) {
    Node* cur = root;
    while (cur != nullptr) {
        if (id == cur->data.id) return &cur->data;
        cur = (id < cur->data.id) ? cur->left : cur->right;
    }
    return nullptr;
}

const Order* OrderBST::search(int id) const {
    const Node* cur = root;
    while (cur != nullptr) {
        if (id == cur->data.id) return &cur->data;
        cur = (id < cur->data.id) ? cur->left : cur->right;
    }
    return nullptr;
}

int OrderBST::heightOf(const Node* n) {
    if (n == nullptr) return 0;
    int lh = heightOf(n->left), rh = heightOf(n->right);
    return 1 + (lh > rh ? lh : rh);
}
