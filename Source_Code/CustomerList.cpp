// CustomerList.cpp
// CS212 Project 94 - Tailoring Order Workshop (simplified course-style version)

#include <string>
#include "CustomerList.h"

using namespace std;

void listInit(CustomerList& list) {
    list.head = NULL;
    list.tail = NULL;
    list.count = 0;
}

void listClear(CustomerList& list) {
    while (list.head != NULL) {
        CustomerNode* old = list.head;
        list.head = list.head->next;
        delete old;
    }
    list.tail = NULL;
    list.count = 0;
}

Customer* listSearch(CustomerList& list, int id) {
    for (CustomerNode* cur = list.head; cur != NULL; cur = cur->next)
        if (cur->data.id == id) return &cur->data;
    return NULL;
}

bool listInsert(CustomerList& list, Customer& c) {
    if (listSearch(list, c.id) != NULL) return false;   // duplicate ID: reject
    CustomerNode* node = new CustomerNode;
    node->data = c;
    node->next = NULL;
    if (list.head == NULL) {
        list.head = node;
        list.tail = node;
    } else {
        list.tail->next = node;
        list.tail = node;
    }
    list.count++;
    return true;
}

bool listRemove(CustomerList& list, int id) {
    CustomerNode* prev = NULL;
    CustomerNode* cur = list.head;
    while (cur != NULL && cur->data.id != id) {
        prev = cur;
        cur = cur->next;
    }
    if (cur == NULL) return false;            // not found (also covers an empty list)

    if (prev == NULL) list.head = cur->next;  // deleting the first node
    else prev->next = cur->next;

    if (cur == list.tail) list.tail = prev;      // deleting the last node
    delete cur;
    list.count--;
    return true;                                 // only-node case: head = tail = NULL
}

// ----------------------------- merge sort ---------------------------------

// Lower-case copy of a string (so "ahmed" and "Ahmed" sort together).
string lowerName(string s) {
    string r = s;
    for (int i = 0; i < (int)r.size(); i++)
        if (r[i] >= 'A' && r[i] <= 'Z') r[i] = r[i] + ('a' - 'A');
    return r;
}

// true if a must come before b
bool lessThan(Customer& a, Customer& b, int sortKey) {
    if (sortKey == 2 /*by ID*/) return a.id < b.id;
    string na = lowerName(a.name), nb = lowerName(b.name);
    if (na != nb) return na < nb;
    return a.id < b.id;   // tie-break so the order is fully deterministic
}

// Splits the chain in half (walks to the middle); returns the second half.
CustomerNode* splitList(CustomerNode* start) {
    int len = 0;
    for (CustomerNode* p = start; p != NULL; p = p->next) len++;
    CustomerNode* mid = start;
    for (int i = 1; i < len / 2; i++) mid = mid->next;   // last node of the first half
    CustomerNode* second = mid->next;
    mid->next = NULL;
    return second;
}

// Merges two sorted chains with a loop. Takes from 'a' on ties (keeps it stable).
CustomerNode* mergeLists(CustomerNode* a, CustomerNode* b, int sortKey) {
    CustomerNode* result = NULL;   // first node of the merged chain
    CustomerNode* last = NULL;     // last node added so far
    while (a != NULL && b != NULL) {
        CustomerNode* pick;
        if (lessThan(b->data, a->data, sortKey)) {
            pick = b;
            b = b->next;
        } else {
            pick = a;
            a = a->next;
        }
        if (result == NULL) result = pick;
        else last->next = pick;
        last = pick;
    }
    CustomerNode* rest = b;
    if (a != NULL) rest = a;
    if (result == NULL) return rest;
    last->next = rest;
    return result;
}

CustomerNode* mergeSort(CustomerNode* start, int sortKey) {
    if (start == NULL || start->next == NULL) return start;   // 0 or 1 node
    CustomerNode* second = splitList(start);
    start = mergeSort(start, sortKey);
    second = mergeSort(second, sortKey);
    return mergeLists(start, second, sortKey);
}

void listSort(CustomerList& list, int sortKey) {
    list.head = mergeSort(list.head, sortKey);
    list.tail = list.head;                       // re-establish the tail pointer
    while (list.tail != NULL && list.tail->next != NULL) list.tail = list.tail->next;
}
