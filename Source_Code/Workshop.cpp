// Workshop.cpp
// CS212 Project 94 - Tailoring Order Workshop (simplified course-style version)

#include <iostream>
#include <string>
#include "Workshop.h"
#include "CustomerList.h"
#include "OrderBST.h"
#include "Queue.h"
#include "Stack.h"
#include "InputUtils.h"

using namespace std;

// ---- the four data structures (one of each) ----
CustomerList customers;   // linked list
OrderBST orders;          // binary search tree
Queue sewingQueue;        // circular array queue
Stack finished;           // linked stack

void initWorkshop() {
    listInit(customers);
    bstInit(orders);
    queueInit(sewingQueue);
    stackInit(finished);
}

void closeWorkshop() {
    listClear(customers);
    bstClear(orders);
    stackClear(finished);
}

// =========================== display helpers ==============================

// Adds spaces to the right of text until it is 'width' characters wide.
string padRight(string text, int width) {
    string r = text;
    while ((int)r.size() < width) r += " ";
    return r;
}

// Text of a number with a fixed count of decimals, e.g. fixedText(1650, 2) = "1650.00".
// (Only used for values that are never negative.)
string fixedText(double value, int decimals) {
    int scale = 1;
    for (int i = 0; i < decimals; i++) scale = scale * 10;
    int whole = (int)value;
    int part = (int)((value - whole) * scale + 0.5);
    if (part >= scale) {
        whole++;
        part = part - scale;
    }
    string frac = intToText(part);
    while ((int)frac.size() < decimals) frac = "0" + frac;
    return intToText(whole) + "." + frac;
}

void printCustomer(Customer& c) {
    cout << "  #" << padRight(intToText(c.id), 6) << padRight(c.name, 20) << padRight(c.phone, 14)
         << " chest " << fixedText(c.m.chest, 1) << " | waist " << fixedText(c.m.waist, 1)
         << " | hips " << fixedText(c.m.hips, 1) << " | inseam " << fixedText(c.m.inseam, 1)
         << " | sleeve " << fixedText(c.m.sleeve, 1) << " (cm)\n";
}

void printOrderHeader() {
    cout << "  " << padRight("ID", 7) << padRight("Cust", 7) << padRight("Garment", 18)
         << padRight("Alteration", 32) << padRight("Price", 10) << "Status\n"
         << "  " << string(84, '-') << "\n";
}

void printOrderRow(Order& o) {
    cout << "  " << padRight(intToText(o.id), 7) << padRight(intToText(o.customerId), 7)
         << padRight(o.garment.substr(0, 17), 18) << padRight(o.alteration.substr(0, 31), 32)
         << padRight(fixedText(o.price, 2), 10) << statusToString(o.status) << "\n";
}

// ===================== recursive walks over the order tree =================
// (same pattern as the BST traversal examples: visit left, node, right)

void printInorder(BSTNode* n) {
    if (n == NULL) return;
    printInorder(n->left);
    printOrderRow(n->data);
    printInorder(n->right);
}

void printPreorder(BSTNode* n, int depth) {
    if (n == NULL) return;
    cout << "  " << string(depth * 4, ' ') << "[" << n->data.id << "] "
         << n->data.garment << " (" << statusToString(n->data.status) << ")\n";
    printPreorder(n->left, depth + 1);
    printPreorder(n->right, depth + 1);
}

int countOrdersOf(BSTNode* n, int customerId) {
    if (n == NULL) return 0;
    int here = 0;
    if (n->data.customerId == customerId) here = 1;
    return here + countOrdersOf(n->left, customerId) + countOrdersOf(n->right, customerId);
}

void printOrdersOf(BSTNode* n, int customerId) {
    if (n == NULL) return;
    printOrdersOf(n->left, customerId);
    if (n->data.customerId == customerId) printOrderRow(n->data);
    printOrdersOf(n->right, customerId);
}

struct Totals {
    int waiting;
    int completed;
    int pickedUp;
    double revenue;
    bool customersOk;     // every order's customer exists
};

void collectTotals(BSTNode* n, Totals& t) {
    if (n == NULL) return;
    collectTotals(n->left, t);
    if (n->data.status == 0 /*WAITING*/) t.waiting++;
    else if (n->data.status == 1 /*COMPLETED*/) t.completed++;
    else { t.pickedUp++; t.revenue += n->data.price; }
    if (listSearch(customers, n->data.customerId) == NULL) t.customersOk = false;   // I5
    collectTotals(n->right, t);
}

// ============================== customers =================================

bool addCustomer(Customer& c) {
    if (!listInsert(customers, c)) {
        cout << "  Error: customer ID " << c.id << " already exists. Duplicate IDs are not allowed.\n";
        return false;
    }
    cout << "  Customer #" << c.id << " (" << c.name << ") added. Customers on file: "
         << customers.count << "\n";
    return true;
}

void listCustomers() {
    if (customers.head == NULL) {
        cout << "  No customers on file yet.\n";
        return;
    }
    cout << "  Customers (" << customers.count << "), in list order:\n";
    for (CustomerNode* cur = customers.head; cur != NULL; cur = cur->next)
        printCustomer(cur->data);
}

bool showCustomer(int id) {
    Customer* c = listSearch(customers, id);
    if (c == NULL) {
        cout << "  Error: no customer with ID " << id << ".\n";
        return false;
    }
    printCustomer(*c);
    int n = countOrdersOf(orders.root, id);
    if (n == 0) {
        cout << "  This customer has no orders.\n";
    } else {
        cout << "  Orders for this customer (" << n << "):\n";
        printOrderHeader();
        printOrdersOf(orders.root, id);
    }
    return true;
}

Customer* findCustomer(int id) { return listSearch(customers, id); }

bool deleteCustomer(int id) {
    if (listSearch(customers, id) == NULL) {
        cout << "  Error: no customer with ID " << id << ".\n";
        return false;
    }
    int n = countOrdersOf(orders.root, id);
    if (n > 0) {
        cout << "  Error: customer #" << id << " still has " << n
             << " order(s). Delete those orders first to keep records consistent.\n";
        return false;
    }
    listRemove(customers, id);
    cout << "  Customer #" << id << " deleted. Customers on file: " << customers.count << "\n";
    return true;
}

void sortCustomers(int sortKey) {
    if (customers.head == NULL) {
        cout << "  No customers to sort.\n";
        return;
    }
    listSort(customers, sortKey);
    string keyName = "ID";
    if (sortKey == 1 /*by name*/) keyName = "name";
    cout << "  Merge sort complete (" << customers.count << " customers, by "
         << keyName << "):\n";
    for (CustomerNode* cur = customers.head; cur != NULL; cur = cur->next)
        printCustomer(cur->data);
}

// ============== queue / stack helpers (only use the ADT operations) =========

int positionInQueue(int id) {
    for (int i = 0; i < queueSize(sewingQueue); i++)
        if (queueAt(sewingQueue, i) == id) return i + 1;
    return -1;
}

// Removes id from the queue, keeping the order of the others.
bool removeFromQueue(int id) {
    bool found = false;
    int n = queueSize(sewingQueue);
    for (int i = 0; i < n; i++) {
        int x = 0;
        dequeue(sewingQueue, x);
        if (x == id && !found) found = true;      // drop it
        else enqueue(sewingQueue, x);             // put the rest back at the rear
    }
    return found;
}

// Replaces oldId by newId in the queue, in the same position.
bool replaceInQueue(int oldId, int newId) {
    bool found = false;
    int n = queueSize(sewingQueue);
    for (int i = 0; i < n; i++) {
        int x = 0;
        dequeue(sewingQueue, x);
        if (x == oldId) { x = newId; found = true; }
        enqueue(sewingQueue, x);
    }
    return found;
}

// Removes id from the stack: pop onto a temporary stack, then restore.
bool removeFromStack(int id) {
    Stack temp;
    stackInit(temp);
    bool found = false;
    int x = 0;
    while (stackPop(finished, x)) {
        if (x == id) { found = true; break; }
        stackPush(temp, x);
    }
    while (stackPop(temp, x)) stackPush(finished, x);   // restore original order
    return found;
}

// Replaces oldId by newId in the stack, in the same position.
bool replaceInStack(int oldId, int newId) {
    Stack temp;
    stackInit(temp);
    bool found = false;
    int x = 0;
    while (stackPop(finished, x)) {
        if (x == oldId) { stackPush(finished, newId); found = true; break; }
        stackPush(temp, x);
    }
    while (stackPop(temp, x)) stackPush(finished, x);
    return found;
}

// ================================ orders ==================================

bool createOrder(Order& o) {
    if (listSearch(customers, o.customerId) == NULL) {
        cout << "  Error: customer ID " << o.customerId
             << " does not exist. Add the customer first.\n";
        return false;
    }
    if (bstSearch(orders, o.id) != NULL) {
        cout << "  Error: order ID " << o.id << " already exists. Duplicate IDs are rejected.\n";
        return false;
    }
    if (queueIsFull(sewingQueue)) {   // check before touching the BST so nothing is half-done
        cout << "  Error: sewing rack is full (" << sewingQueue.capacity
             << " garments). Sew some garments before accepting new orders.\n";
        return false;
    }
    Order copy = o;
    copy.status = 0 /*WAITING*/;
    bstInsert(orders, copy);
    enqueue(sewingQueue, copy.id);
    cout << "  Order #" << copy.id << " created and placed in the sewing queue (position "
         << queueSize(sewingQueue) << " of " << queueSize(sewingQueue) << ").\n";
    return true;
}

bool showOrder(int id) {
    Order* o = bstSearch(orders, id);
    if (o == NULL) {
        cout << "  Error: no order with ID " << id << ".\n";
        return false;
    }
    printOrderHeader();
    printOrderRow(*o);
    Customer* c = listSearch(customers, o->customerId);
    if (c != NULL) cout << "  Customer: " << c->name << " (" << c->phone << ")\n";
    if (o->status == 0 /*WAITING*/)
        cout << "  Location: sewing queue, position " << positionInQueue(id) << " of "
             << queueSize(sewingQueue) << "\n";
    else if (o->status == 1 /*COMPLETED*/)
        cout << "  Location: finished rack, waiting for pickup\n";
    else
        cout << "  Location: delivered to customer\n";
    return true;
}

Order* findOrder(int id) { return bstSearch(orders, id); }

void listOrdersInorder() {
    if (orders.root == NULL) {
        cout << "  No orders in the system.\n";
        return;
    }
    cout << "  All orders, inorder traversal (ascending ID) - " << orders.count << " total:\n";
    printOrderHeader();
    printInorder(orders.root);
}

void showTreePreorder() {
    if (orders.root == NULL) {
        cout << "  The order tree is empty.\n";
        return;
    }
    cout << "  Preorder traversal (indent = depth). Nodes: " << orders.count
         << ", height: " << bstHeight(orders) << "\n";
    printPreorder(orders.root, 0);
}

bool updateOrder(int id, string* garment, string* alteration,
                 double* price, int* customerId) {
    Order* o = bstSearch(orders, id);
    if (o == NULL) {
        cout << "  Error: no order with ID " << id << ".\n";
        return false;
    }
    if (o->status == 2 /*PICKED_UP*/) {
        cout << "  Error: order #" << id << " was already picked up; archived orders cannot be edited.\n";
        return false;
    }
    if (customerId != NULL && listSearch(customers, *customerId) == NULL) {
        cout << "  Error: customer ID " << *customerId << " does not exist. No changes made.\n";
        return false;
    }
    if (garment != NULL) o->garment = *garment;
    if (alteration != NULL) o->alteration = *alteration;
    if (price != NULL) o->price = *price;
    if (customerId != NULL) o->customerId = *customerId;
    cout << "  Order #" << id << " updated:\n";
    printOrderHeader();
    printOrderRow(*o);
    return true;
}

// Scenario: an order was registered under the wrong ID. The key changes, so the
// BST node must be removed and re-inserted, and the copy of the ID held in the
// queue or stack is rewritten IN PLACE so the order keeps its position.
bool changeOrderId(int oldId, int newId) {
    Order* o = bstSearch(orders, oldId);
    if (o == NULL) {
        cout << "  Error: no order with ID " << oldId << ".\n";
        return false;
    }
    if (oldId == newId) {
        cout << "  Error: the new ID is the same as the old ID. Nothing changed.\n";
        return false;
    }
    if (bstSearch(orders, newId) != NULL) {
        cout << "  Error: order ID " << newId << " is already in use. Duplicate IDs are rejected.\n";
        return false;
    }
    Order copy = *o;
    copy.id = newId;
    bstRemove(orders, oldId);
    bstInsert(orders, copy);

    string where = "BST only (order is picked up)";
    if (copy.status == 0 /*WAITING*/) {
        replaceInQueue(oldId, newId);
        where = "BST + sewing queue (position " + intToText(positionInQueue(newId)) + " kept)";
    } else if (copy.status == 1 /*COMPLETED*/) {
        replaceInStack(oldId, newId);
        where = "BST + finished stack (position kept)";
    }
    cout << "  Order #" << oldId << " is now #" << newId << ". Updated: " << where << ".\n";
    return true;
}

bool deleteOrder(int id) {
    Order* o = bstSearch(orders, id);
    if (o == NULL) {
        cout << "  Error: no order with ID " << id << ".\n";
        return false;
    }
    string where = "BST";
    if (o->status == 0 /*WAITING*/) {
        removeFromQueue(id);
        where += " + sewing queue";
    } else if (o->status == 1 /*COMPLETED*/) {
        removeFromStack(id);
        where += " + finished stack";
    }
    bstRemove(orders, id);
    cout << "  Order #" << id << " deleted from " << where << ". Orders remaining: "
         << orders.count << "\n";
    return true;
}

// =============================== workflow =================================

void showQueue() {
    if (queueIsEmpty(sewingQueue)) {
        cout << "  The sewing queue is empty (0/" << sewingQueue.capacity << ").\n";
        return;
    }
    cout << "  Sewing queue (" << queueSize(sewingQueue) << "/" << sewingQueue.capacity
         << "), front first:\n";
    for (int i = 0; i < queueSize(sewingQueue); i++) {
        int id = queueAt(sewingQueue, i);
        Order* o = bstSearch(orders, id);
        cout << "   " << (i + 1) << ". #" << id;
        if (o != NULL) cout << "  " << o->garment << " - " << o->alteration;
        cout << "\n";
    }
}

bool sewNext() {
    int id = 0;
    if (!dequeue(sewingQueue, id)) {
        cout << "  Nothing to sew: the sewing queue is empty.\n";
        return false;
    }
    Order* o = bstSearch(orders, id);
    if (o == NULL) {   // cannot happen while the invariants hold
        cout << "  Internal error: queued order #" << id << " not found in BST.\n";
        return false;
    }
    o->status = 1 /*COMPLETED*/;
    stackPush(finished, id);
    cout << "  Sewed order #" << id << " (" << o->garment << ": " << o->alteration << ").\n"
         << "  Status -> COMPLETED. Pushed onto finished stack (size " << stackSize(finished)
         << "). Queue now holds " << queueSize(sewingQueue) << ".\n";
    return true;
}

void showFinished() {
    if (stackIsEmpty(finished)) {
        cout << "  No finished alterations are waiting for pickup.\n";
        return;
    }
    cout << "  Recently finished alterations (" << stackSize(finished) << "), most recent first:\n";
    int pos = 1;
    for (StackNode* cur = finished.top; cur != NULL; cur = cur->next) {
        Order* o = bstSearch(orders, cur->data);
        cout << "   " << pos << ". #" << cur->data;
        if (o != NULL) cout << "  " << o->garment << " - " << o->alteration;
        if (pos == 1) cout << "   <- top";
        cout << "\n";
        pos++;
    }
}

// Scenario: quality control rejects the most recently finished garment (LIFO).
bool reworkLatest() {
    int id = 0;
    if (!stackPeek(finished, id)) {
        cout << "  Nothing to rework: the finished stack is empty.\n";
        return false;
    }
    if (queueIsFull(sewingQueue)) {
        cout << "  Error: sewing queue is full; cannot send order #" << id
             << " back for rework right now.\n";
        return false;
    }
    stackPop(finished, id);
    Order* o = bstSearch(orders, id);
    o->status = 0 /*WAITING*/;
    enqueue(sewingQueue, id);
    cout << "  Order #" << id << " failed quality check: popped from finished stack and "
         << "re-queued for sewing (position " << queueSize(sewingQueue) << ").\n";
    return true;
}

bool pickUp(int id) {
    Order* o = bstSearch(orders, id);
    if (o == NULL) {
        cout << "  Error: no order with ID " << id << ".\n";
        return false;
    }
    if (o->status == 0 /*WAITING*/) {
        cout << "  Error: order #" << id << " is not sewn yet (queue position "
             << positionInQueue(id) << ").\n";
        return false;
    }
    if (o->status == 2 /*PICKED_UP*/) {
        cout << "  Error: order #" << id << " was already picked up.\n";
        return false;
    }
    removeFromStack(id);
    o->status = 2 /*PICKED_UP*/;
    cout << "  Order #" << id << " handed to the customer. Amount due: " << fixedText(o->price, 2)
         << " EGP. Status -> PICKED_UP.\n";
    return true;
}

void showSummary() {
    string fullFlag = "";
    if (queueIsFull(sewingQueue)) fullFlag = "  (FULL)";
    Totals t;
    t.waiting = 0;
    t.completed = 0;
    t.pickedUp = 0;
    t.revenue = 0.0;
    t.customersOk = true;
    collectTotals(orders.root, t);
    cout << "  ===== Workshop summary =====\n"
         << "  Customers (linked list) : " << customers.count << "\n"
         << "  Orders (BST)            : " << orders.count << "   tree height " << bstHeight(orders) << "\n"
         << "  Sewing queue            : " << queueSize(sewingQueue) << "/" << sewingQueue.capacity
         << fullFlag << "\n"
         << "  Finished stack          : " << stackSize(finished) << "\n"
         << "  Status counts           : WAITING " << t.waiting << ", COMPLETED " << t.completed
         << ", PICKED_UP " << t.pickedUp << "\n"
         << "  Revenue collected       : " << fixedText(t.revenue, 2) << " EGP\n";
    checkConsistency(true);
}

bool checkConsistency(bool verbose) {
    bool ok = true;
    Totals t;
    t.waiting = 0;
    t.completed = 0;
    t.pickedUp = 0;
    t.revenue = 0.0;
    t.customersOk = true;
    collectTotals(orders.root, t);
    if (!t.customersOk) ok = false;                                                    // I5
    if (t.waiting != queueSize(sewingQueue) || t.completed != stackSize(finished)) ok = false;  // I2, I3
    for (int i = 0; i < queueSize(sewingQueue); i++) {
        Order* o = bstSearch(orders, queueAt(sewingQueue, i));
        if (o == NULL || o->status != 0 /*WAITING*/) ok = false;
    }
    for (StackNode* cur = finished.top; cur != NULL; cur = cur->next) {
        Order* o = bstSearch(orders, cur->data);
        if (o == NULL || o->status != 1 /*COMPLETED*/) ok = false;
    }
    if (verbose) {
        string verdict = "FAILED";
        if (ok) verdict = "OK";
        cout << "  Consistency check       : " << verdict << "\n";
    }
    return ok;
}

// ============================== sample data ================================

Customer makeCustomer(int id, string name, string phone,
                             double chest, double waist, double hips, double inseam, double sleeve) {
    Customer c;
    c.id = id;
    c.name = name;
    c.phone = phone;
    c.m.chest = chest;
    c.m.waist = waist;
    c.m.hips = hips;
    c.m.inseam = inseam;
    c.m.sleeve = sleeve;
    return c;
}

Order makeOrder(int id, int customerId, string garment, string alteration,
                       double price) {
    Order o;
    o.id = id;
    o.customerId = customerId;
    o.garment = garment;
    o.alteration = alteration;
    o.price = price;
    o.status = 0 /*WAITING*/;
    return o;
}

void loadSampleData() {
    cout << "  Loading sample data (duplicates are rejected, so loading twice is safe)...\n";
    { Customer tmpC = makeCustomer(101, "Mona Hassan", "01001234567", 92, 74, 98, 78, 58); addCustomer(tmpC); }
    { Customer tmpC = makeCustomer(102, "Ahmed Fathy", "01112223334", 104, 90, 100, 82, 63); addCustomer(tmpC); }
    { Customer tmpC = makeCustomer(103, "Sara Adel", "01223344556", 86, 68, 94, 76, 56); addCustomer(tmpC); }
    { Customer tmpC = makeCustomer(104, "Karim Nabil", "01556677889", 110, 96, 104, 84, 65); addCustomer(tmpC); }
    { Order tmpO = makeOrder(5005, 102, "Suit jacket", "Shorten sleeves 2 cm", 350.0); createOrder(tmpO); }
    { Order tmpO = makeOrder(3003, 101, "Evening dress", "Take in waist 3 cm", 420.0); createOrder(tmpO); }
    { Order tmpO = makeOrder(7007, 103, "Jeans", "Hem to 76 cm inseam", 120.0); createOrder(tmpO); }
    { Order tmpO = makeOrder(2002, 104, "Shirt", "Slim fit sides", 180.0); createOrder(tmpO); }
    { Order tmpO = makeOrder(6006, 101, "Coat", "Replace lining", 650.0); createOrder(tmpO); }
}
