// CustomerList.h
// CS212 Project 94 - Tailoring Order Workshop
// Manually implemented singly linked list of customers and their measurements.
// Operations: insert, delete (remove), search, traverse, plus merge sort.
#ifndef CUSTOMER_LIST_H
#define CUSTOMER_LIST_H

#include "Customer.h"

enum class CustomerSortKey { BY_NAME, BY_ID };

class CustomerList {
private:
    struct Node {
        Customer data;
        Node* next;
        explicit Node(const Customer& c) : data(c), next(nullptr) {}
    };
    Node* head;
    Node* tail;
    int count;

    // Merge sort helpers (operate on node chains, no extra arrays).
    static bool lessThan(const Customer& a, const Customer& b, CustomerSortKey key);
    static Node* split(Node* start);
    static Node* merge(Node* a, Node* b, CustomerSortKey key);
    static Node* mergeSort(Node* start, CustomerSortKey key);

public:
    CustomerList();
    ~CustomerList();
    CustomerList(const CustomerList&) = delete;
    CustomerList& operator=(const CustomerList&) = delete;

    // Appends at the tail. Returns false if the ID already exists. O(n) (dup check)
    bool insert(const Customer& c);

    // Removes the node with this ID (head, middle, tail or only node).
    // Returns false if not found. O(n)
    bool remove(int id);

    // Linear search by ID. Returns nullptr if not found. O(n)
    Customer* search(int id);
    const Customer* search(int id) const;

    // Visits every customer from head to tail. O(n)
    template <typename Visitor>
    void traverse(Visitor visit) const {
        for (Node* cur = head; cur != nullptr; cur = cur->next) visit(cur->data);
    }

    // Stable in-place merge sort of the list. O(n log n) time, O(log n) stack.
    void sort(CustomerSortKey key);

    bool isEmpty() const { return head == nullptr; }
    int size() const { return count; }
};

#endif // CUSTOMER_LIST_H
