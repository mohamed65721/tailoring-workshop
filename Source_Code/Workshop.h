// Workshop.h
// CS212 Project 94 - Tailoring Order Workshop
// Coordinator that owns all four data structures and keeps them consistent.
//
// Invariants maintained by every public operation:
//   I1. Every order lives exactly once in the BST (single source of truth).
//   I2. status == WAITING   <=> the order ID is in the sewing queue (once).
//   I3. status == COMPLETED <=> the order ID is on the finished stack (once).
//   I4. status == PICKED_UP  => the ID is in neither the queue nor the stack.
//   I5. Every order's customerId exists in the customer linked list.
#ifndef WORKSHOP_H
#define WORKSHOP_H

#include "Customer.h"
#include "CustomerList.h"
#include "Order.h"
#include "OrderBST.h"
#include "Queue.h"
#include "Stack.h"

#include <string>

class Workshop {
public:
    static const int QUEUE_CAPACITY = 8;   // sewing rack holds 8 garments
    static const int MIN_ID = 1;
    static const int MAX_ID = 99999;

    Workshop();

    // ---- Customers (linked list) ----
    bool addCustomer(const Customer& c);
    void listCustomers() const;
    bool showCustomer(int id) const;               // details + that customer's orders
    Customer* findCustomer(int id);
    bool deleteCustomer(int id);
    void sortCustomers(CustomerSortKey key);

    // ---- Orders (BST) ----
    bool createOrder(const Order& o);              // BST insert + enqueue
    bool showOrder(int id) const;
    Order* findOrder(int id);
    void listOrdersInorder() const;
    void showTreePreorder() const;
    bool updateOrder(int id, const std::string* garment, const std::string* alteration,
                     const double* price, const int* customerId);
    bool changeOrderId(int oldId, int newId);      // scenario: correction
    bool deleteOrder(int id);                      // removes from every structure

    // ---- Workflow (queue + stack) ----
    void showQueue() const;
    bool sewNext();                                // scenario: dequeue -> complete -> push
    void showFinished() const;
    bool reworkLatest();                           // scenario: pop -> back into queue
    bool pickUp(int id);                           // COMPLETED -> PICKED_UP

    void showSummary() const;
    bool checkConsistency(bool verbose) const;
    void loadSampleData();

    int queueSize() const { return sewingQueue.size(); }
    int finishedSize() const { return finished.size(); }

private:
    CustomerList customers;
    OrderBST orders;
    Queue<int> sewingQueue;
    Stack<int> finished;

    // Helpers built only from the public queue/stack operations.
    bool removeFromQueue(int id);
    bool replaceInQueue(int oldId, int newId);
    int positionInQueue(int id) const;
    bool removeFromStack(int id);
    bool replaceInStack(int oldId, int newId);

    int countOrdersForCustomer(int customerId) const;
    static void printOrderHeader();
    static void printOrderRow(const Order& o);
    static void printCustomer(const Customer& c);
};

#endif // WORKSHOP_H
