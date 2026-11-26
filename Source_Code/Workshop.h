// Workshop.h
// CS212 Project 94 - Tailoring Order Workshop (simplified course-style version)
// The workshop owns all four data structures and keeps them consistent.
//
// Invariants maintained by every operation:
//   I1. Every order lives exactly once in the BST (single source of truth).
//   I2. status == WAITING   <=> the order ID is in the sewing queue (once).
//   I3. status == COMPLETED <=> the order ID is on the finished stack (once).
//   I4. status == PICKED_UP  => the ID is in neither the queue nor the stack.
//   I5. Every order's customerId exists in the customer linked list.
#ifndef WORKSHOP_H
#define WORKSHOP_H

#include <string>
#include "Customer.h"
#include "Order.h"

using namespace std;

void initWorkshop();      // call once at program start
void closeWorkshop();     // frees all memory, call once at the end

// ---- Customers (linked list) ----
bool addCustomer(Customer& c);
void listCustomers();
bool showCustomer(int id);                 // details + that customer's orders
Customer* findCustomer(int id);
bool deleteCustomer(int id);
void sortCustomers(int sortKey);           // SORT_BY_NAME or SORT_BY_ID

// ---- Orders (BST) ----
bool createOrder(Order& o);          // BST insert + enqueue
bool showOrder(int id);
Order* findOrder(int id);
void listOrdersInorder();
void showTreePreorder();
bool updateOrder(int id, string* garment, string* alteration,
                 double* price, int* customerId);
bool changeOrderId(int oldId, int newId);  // scenario: correction
bool deleteOrder(int id);                  // removes from every structure

// ---- Workflow (queue + stack) ----
void showQueue();
bool sewNext();                            // scenario: dequeue -> complete -> push
void showFinished();
bool reworkLatest();                       // scenario: pop -> back into queue
bool pickUp(int id);                       // COMPLETED -> PICKED_UP

void showSummary();
bool checkConsistency(bool verbose);
void loadSampleData();

#endif // WORKSHOP_H
