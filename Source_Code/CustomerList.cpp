// CustomerList.cpp
// CS212 Project 94 - Tailoring Order Workshop
#include "CustomerList.h"

#include <cctype>
#include <string>

CustomerList::CustomerList() : head(nullptr), tail(nullptr), count(0) {}

CustomerList::~CustomerList() {
    while (head != nullptr) {
        Node* old = head;
        head = head->next;
        delete old;
    }
}

bool CustomerList::insert(const Customer& c) {
    if (search(c.id) != nullptr) return false;   // duplicate ID policy: reject
    Node* node = new Node(c);
    if (head == nullptr) {
        head = tail = node;
    } else {
        tail->next = node;
        tail = node;
    }
    ++count;
    return true;
}

bool CustomerList::remove(int id) {
    Node* prev = nullptr;
    Node* cur = head;
    while (cur != nullptr && cur->data.id != id) {
        prev = cur;
        cur = cur->next;
    }
    if (cur == nullptr) return false;            // not found (also covers empty list)

    if (prev == nullptr) head = cur->next;       // deleting the first node
    else prev->next = cur->next;

    if (cur == tail) tail = prev;                // deleting the last node
    delete cur;
    --count;
    return true;                                 // only-node case: head = tail = nullptr
}

Customer* CustomerList::search(int id) {
    for (Node* cur = head; cur != nullptr; cur = cur->next)
        if (cur->data.id == id) return &cur->data;
    return nullptr;
}

const Customer* CustomerList::search(int id) const {
    for (Node* cur = head; cur != nullptr; cur = cur->next)
        if (cur->data.id == id) return &cur->data;
    return nullptr;
}

// ----------------------------- merge sort ---------------------------------

static std::string lowerCopy(const std::string& s) {
    std::string r = s;
    for (char& ch : r) ch = static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
    return r;
}

bool CustomerList::lessThan(const Customer& a, const Customer& b, CustomerSortKey key) {
    if (key == CustomerSortKey::BY_ID) return a.id < b.id;
    std::string na = lowerCopy(a.name), nb = lowerCopy(b.name);
    if (na != nb) return na < nb;
    return a.id < b.id;  // tie-break so the order is fully deterministic
}

// Splits the chain in half using slow/fast pointers; returns the second half.
CustomerList::Node* CustomerList::split(Node* start) {
    Node* slow = start;
    Node* fast = start->next;
    while (fast != nullptr && fast->next != nullptr) {
        slow = slow->next;
        fast = fast->next->next;
    }
    Node* second = slow->next;
    slow->next = nullptr;
    return second;
}

// Merges two sorted chains. Takes from 'a' on ties, which keeps the sort stable.
CustomerList::Node* CustomerList::merge(Node* a, Node* b, CustomerSortKey key) {
    Node dummy{Customer{}};
    Node* tailPtr = &dummy;
    while (a != nullptr && b != nullptr) {
        if (lessThan(b->data, a->data, key)) {
            tailPtr->next = b;
            b = b->next;
        } else {
            tailPtr->next = a;
            a = a->next;
        }
        tailPtr = tailPtr->next;
    }
    tailPtr->next = (a != nullptr) ? a : b;
    return dummy.next;
}

CustomerList::Node* CustomerList::mergeSort(Node* start, CustomerSortKey key) {
    if (start == nullptr || start->next == nullptr) return start;  // 0 or 1 node
    Node* second = split(start);
    return merge(mergeSort(start, key), mergeSort(second, key), key);
}

void CustomerList::sort(CustomerSortKey key) {
    head = mergeSort(head, key);
    tail = head;                                  // re-establish the tail pointer
    while (tail != nullptr && tail->next != nullptr) tail = tail->next;
}
