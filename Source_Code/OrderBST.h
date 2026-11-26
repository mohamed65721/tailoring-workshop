// OrderBST.h
// CS212 Project 94 - Tailoring Order Workshop (simplified course-style version)
// Binary search tree of orders keyed by unique order ID, written by hand.
// The BST is the single source of truth for order details.
//
// Duplicate-ID policy: IDs are unique. bstInsert() REJECTS a duplicate ID and
// returns false; the existing order is left unchanged.
#ifndef ORDERBST_H
#define ORDERBST_H

#include <cstddef>
#include "Order.h"

struct BSTNode {
    Order data;
    BSTNode* left;
    BSTNode* right;
};

struct OrderBST {
    BSTNode* root;
    int count;
};

void bstInit(OrderBST& t);
void bstClear(OrderBST& t);

// Recursive insert. Returns false if the ID already exists (duplicate rejected).
bool bstInsert(OrderBST& t, Order& o);

// Recursive delete (leaf, one child, two children). Returns false if not found.
bool bstRemove(OrderBST& t, int id);

// Recursive search. Returns NULL if not found.
Order* bstSearch(OrderBST& t, int id);

int bstHeight(OrderBST& t);   // empty = 0, single node = 1

#endif // ORDERBST_H
