// OrderBST.cpp
// CS212 Project 94 - Tailoring Order Workshop (simplified course-style version)

#include "OrderBST.h"

void bstInit(OrderBST& t) {
    t.root = NULL;
    t.count = 0;
}

void destroyNode(BSTNode* n) {
    if (n == NULL) return;
    destroyNode(n->left);       // post-order: children first
    destroyNode(n->right);
    delete n;
}

void bstClear(OrderBST& t) {
    destroyNode(t.root);
    t.root = NULL;
    t.count = 0;
}

// Recursive insert: returns the (possibly new) root of this subtree.
BSTNode* insertNode(BSTNode* root, Order& o, bool& inserted) {
    if (root == NULL) {
        BSTNode* node = new BSTNode;
        node->data = o;
        node->left = NULL;
        node->right = NULL;
        inserted = true;
        return node;
    }
    if (o.id == root->data.id) {          // duplicate ID -> reject
        inserted = false;
        return root;
    }
    if (o.id < root->data.id) root->left = insertNode(root->left, o, inserted);
    else root->right = insertNode(root->right, o, inserted);
    return root;
}

bool bstInsert(OrderBST& t, Order& o) {
    bool inserted = false;
    t.root = insertNode(t.root, o, inserted);
    if (inserted) t.count++;
    return inserted;
}

// Smallest node of a subtree (leftmost). Used as the inorder successor.
BSTNode* getSuccessor(BSTNode* curr) {
    curr = curr->right;
    while (curr != NULL && curr->left != NULL) curr = curr->left;
    return curr;
}

// Recursive delete. Handles: leaf, one child, two children (inorder successor),
// and the root-only tree (returns NULL so the root becomes empty).
BSTNode* deleteNode(BSTNode* root, int id, bool& removed) {
    if (root == NULL) return root;
    if (id < root->data.id) {
        root->left = deleteNode(root->left, id, removed);
    } else if (id > root->data.id) {
        root->right = deleteNode(root->right, id, removed);
    } else {
        removed = true;
        if (root->left == NULL) {          // 0 or 1 child (right)
            BSTNode* temp = root->right;
            delete root;
            return temp;
        }
        if (root->right == NULL) {         // 1 child (left)
            BSTNode* temp = root->left;
            delete root;
            return temp;
        }
        BSTNode* succ = getSuccessor(root);   // 2 children
        root->data = succ->data;
        bool dummy = false;
        root->right = deleteNode(root->right, succ->data.id, dummy);
    }
    return root;
}

bool bstRemove(OrderBST& t, int id) {
    bool removed = false;
    t.root = deleteNode(t.root, id, removed);
    if (removed) t.count--;
    return removed;
}

Order* searchNode(BSTNode* root, int id) {
    if (root == NULL) return NULL;
    if (id == root->data.id) return &root->data;
    if (id > root->data.id) return searchNode(root->right, id);
    return searchNode(root->left, id);
}

Order* bstSearch(OrderBST& t, int id) {
    return searchNode(t.root, id);
}

int heightNode(BSTNode* n) {
    if (n == NULL) return 0;
    int lh = heightNode(n->left), rh = heightNode(n->right);
    if (lh > rh) return 1 + lh;
    return 1 + rh;
}

int bstHeight(OrderBST& t) {
    return heightNode(t.root);
}
