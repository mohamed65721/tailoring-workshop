// CustomerList.h
// CS212 Project 94 - Tailoring Order Workshop (simplified course-style version)
// Singly linked list of customers and their measurements, written by hand.
// Operations: insert, delete (remove), search, traverse, plus merge sort.
#ifndef CUSTOMERLIST_H
#define CUSTOMERLIST_H

#include <cstddef>
#include "Customer.h"

// SORT_BY_NAME = 1
// SORT_BY_ID = 2

struct CustomerNode {
    Customer data;
    CustomerNode* next;
};

struct CustomerList {
    CustomerNode* head;
    CustomerNode* tail;
    int count;
};

void listInit(CustomerList& list);
void listClear(CustomerList& list);

// Appends at the tail. Returns false if the ID already exists. O(n) (duplicate check)
bool listInsert(CustomerList& list, Customer& c);

// Removes the node with this ID (head, middle, tail or only node).
// Returns false if not found. O(n)
bool listRemove(CustomerList& list, int id);

// Linear search by ID. Returns NULL if not found. O(n)
Customer* listSearch(CustomerList& list, int id);

// Merge sort of the list (stable). O(n log n) time.
void listSort(CustomerList& list, int sortKey);

#endif // CUSTOMERLIST_H
