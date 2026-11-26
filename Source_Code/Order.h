// Order.h
// CS212 Project 94 - Tailoring Order Workshop (simplified course-style version)
// Order record and its workflow status. The BST holds the only copy of each
// order; the queue and the stack hold order IDs that point back into the BST.
#ifndef ORDER_H
#define ORDER_H

#include <string>

using namespace std;

// STATUS_WAITING = 0    // in the sewing queue
// STATUS_COMPLETED = 1  // sewn, on the "recently finished" stack, awaiting pickup
// STATUS_PICKED_UP = 2  // handed to the customer; in neither queue nor stack

struct Order {
    int id;                 // unique key (1..99999)
    int customerId;         // must exist in the customer linked list
    string garment;         // e.g. "Suit jacket"
    string alteration;      // e.g. "Shorten sleeves 2 cm"
    double price;           // in EGP, 0..100000
    int status;             // STATUS_WAITING, STATUS_COMPLETED or STATUS_PICKED_UP
};

string statusToString(int s);

#endif // ORDER_H
