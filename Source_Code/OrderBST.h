// OrderBST.h
// CS212 Project 94 - Tailoring Order Workshop
// Manually implemented binary search tree of orders keyed by unique order ID.
// The BST is the single source of truth for order details.
//
// Duplicate-ID policy: IDs are unique. insert() REJECTS a duplicate ID and
// returns false; the existing order is left unchanged.
#ifndef ORDER_BST_H
#define ORDER_BST_H

#include "Order.h"

class OrderBST {
private:
    struct Node {
        Order data;
        Node* left;
        Node* right;
        explicit Node(const Order& o) : data(o), left(nullptr), right(nullptr) {}
    };
    Node* root;
    int count;

    static void destroy(Node* n);
    static Node* removeNode(Node* n, int id, bool& removed);
    static Node* minNode(Node* n);
    static int heightOf(const Node* n);

    template <typename Visitor>
    static void inorderRec(const Node* n, Visitor& visit) {
        if (n == nullptr) return;
        inorderRec(n->left, visit);
        visit(n->data);
        inorderRec(n->right, visit);
    }

    template <typename Visitor>
    static void preorderRec(const Node* n, int depth, Visitor& visit) {
        if (n == nullptr) return;
        visit(n->data, depth);
        preorderRec(n->left, depth + 1, visit);
        preorderRec(n->right, depth + 1, visit);
    }

public:
    OrderBST();
    ~OrderBST();
    OrderBST(const OrderBST&) = delete;
    OrderBST& operator=(const OrderBST&) = delete;

    // O(h). Returns false if the ID already exists (duplicate rejected).
    bool insert(const Order& o);

    // O(h). Returns false if the ID is not in the tree.
    bool remove(int id);

    // O(h). Returns nullptr if not found.
    Order* search(int id);
    const Order* search(int id) const;

    // Inorder: visits orders in ascending ID order. O(n)
    template <typename Visitor>
    void inorder(Visitor visit) const { inorderRec(root, visit); }

    // Preorder: root, left, right; also passes the depth for an indented view. O(n)
    template <typename Visitor>
    void preorder(Visitor visit) const { preorderRec(root, 0, visit); }

    bool isEmpty() const { return root == nullptr; }
    int size() const { return count; }
    int height() const { return heightOf(root); }   // empty = 0, single node = 1
};

#endif // ORDER_BST_H
