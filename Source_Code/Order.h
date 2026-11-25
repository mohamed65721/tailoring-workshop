// Order.h
// CS212 Project 94 - Tailoring Order Workshop
// Order record and its workflow status. The BST owns the only copy of each
// Order; the queue and stack hold order IDs that point back into the BST.
#ifndef ORDER_H
#define ORDER_H

#include <string>

enum class OrderStatus {
    WAITING,    // in the sewing queue
    COMPLETED,  // sewn, on the "recently finished" stack, awaiting pickup
    PICKED_UP   // handed to the customer; in neither queue nor stack
};

inline std::string statusToString(OrderStatus s) {
    switch (s) {
        case OrderStatus::WAITING:   return "WAITING";
        case OrderStatus::COMPLETED: return "COMPLETED";
        case OrderStatus::PICKED_UP: return "PICKED_UP";
    }
    return "UNKNOWN";
}

struct Order {
    int id = 0;                 // unique key (1..99999)
    int customerId = 0;         // must exist in the customer linked list
    std::string garment;        // e.g. "Suit jacket"
    std::string alteration;     // e.g. "Shorten sleeves 2 cm"
    double price = 0.0;         // in EGP, 0..100000
    OrderStatus status = OrderStatus::WAITING;
};

#endif // ORDER_H
