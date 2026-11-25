// Workshop.cpp
// CS212 Project 94 - Tailoring Order Workshop
#include "Workshop.h"

#include <iomanip>
#include <iostream>

using std::cout;

Workshop::Workshop() : sewingQueue(QUEUE_CAPACITY) {}

// =========================== display helpers ==============================

void Workshop::printCustomer(const Customer& c) {
    cout << "  #" << std::left << std::setw(6) << c.id << std::setw(20) << c.name
         << std::setw(14) << c.phone << std::right << std::fixed << std::setprecision(1)
         << " chest " << c.m.chest << " | waist " << c.m.waist << " | hips " << c.m.hips
         << " | inseam " << c.m.inseam << " | sleeve " << c.m.sleeve << " (cm)\n";
}

void Workshop::printOrderHeader() {
    cout << "  " << std::left << std::setw(7) << "ID" << std::setw(7) << "Cust"
         << std::setw(18) << "Garment" << std::setw(32) << "Alteration"
         << std::setw(10) << "Price" << "Status\n"
         << "  " << std::string(84, '-') << "\n" << std::right;
}

void Workshop::printOrderRow(const Order& o) {
    cout << "  " << std::left << std::setw(7) << o.id << std::setw(7) << o.customerId
         << std::setw(18) << o.garment.substr(0, 17) << std::setw(32) << o.alteration.substr(0, 31)
         << std::setw(10) << std::fixed << std::setprecision(2) << o.price
         << statusToString(o.status) << "\n" << std::right;
}

// ============================== customers =================================

bool Workshop::addCustomer(const Customer& c) {
    if (!customers.insert(c)) {
        cout << "  Error: customer ID " << c.id << " already exists. Duplicate IDs are not allowed.\n";
        return false;
    }
    cout << "  Customer #" << c.id << " (" << c.name << ") added. Customers on file: "
         << customers.size() << "\n";
    return true;
}

void Workshop::listCustomers() const {
    if (customers.isEmpty()) {
        cout << "  No customers on file yet.\n";
        return;
    }
    cout << "  Customers (" << customers.size() << "), in list order:\n";
    customers.traverse([](const Customer& c) { printCustomer(c); });
}

bool Workshop::showCustomer(int id) const {
    const Customer* c = customers.search(id);
    if (c == nullptr) {
        cout << "  Error: no customer with ID " << id << ".\n";
        return false;
    }
    printCustomer(*c);
    int n = countOrdersForCustomer(id);
    if (n == 0) {
        cout << "  This customer has no orders.\n";
    } else {
        cout << "  Orders for this customer (" << n << "):\n";
        printOrderHeader();
        orders.inorder([id](const Order& o) { if (o.customerId == id) printOrderRow(o); });
    }
    return true;
}

Customer* Workshop::findCustomer(int id) { return customers.search(id); }

bool Workshop::deleteCustomer(int id) {
    if (customers.search(id) == nullptr) {
        cout << "  Error: no customer with ID " << id << ".\n";
        return false;
    }
    int n = countOrdersForCustomer(id);
    if (n > 0) {
        cout << "  Error: customer #" << id << " still has " << n
             << " order(s). Delete those orders first to keep records consistent.\n";
        return false;
    }
    customers.remove(id);
    cout << "  Customer #" << id << " deleted. Customers on file: " << customers.size() << "\n";
    return true;
}

void Workshop::sortCustomers(CustomerSortKey key) {
    if (customers.isEmpty()) {
        cout << "  No customers to sort.\n";
        return;
    }
    customers.sort(key);
    cout << "  Merge sort complete (" << customers.size() << " customers, by "
         << (key == CustomerSortKey::BY_NAME ? "name" : "ID") << "):\n";
    customers.traverse([](const Customer& c) { printCustomer(c); });
}

int Workshop::countOrdersForCustomer(int customerId) const {
    int n = 0;
    orders.inorder([&n, customerId](const Order& o) { if (o.customerId == customerId) ++n; });
    return n;
}

// ================================ orders ==================================

bool Workshop::createOrder(const Order& o) {
    if (customers.search(o.customerId) == nullptr) {
        cout << "  Error: customer ID " << o.customerId
             << " does not exist. Add the customer first.\n";
        return false;
    }
    if (orders.search(o.id) != nullptr) {
        cout << "  Error: order ID " << o.id << " already exists. Duplicate IDs are rejected.\n";
        return false;
    }
    if (sewingQueue.isFull()) {   // check before touching the BST so nothing is half-done
        cout << "  Error: sewing rack is full (" << QUEUE_CAPACITY
             << " garments). Sew some garments before accepting new orders.\n";
        return false;
    }
    Order copy = o;
    copy.status = OrderStatus::WAITING;
    orders.insert(copy);
    sewingQueue.enqueue(copy.id);
    cout << "  Order #" << copy.id << " created and placed in the sewing queue (position "
         << sewingQueue.size() << " of " << sewingQueue.size() << ").\n";
    return true;
}

bool Workshop::showOrder(int id) const {
    const Order* o = orders.search(id);
    if (o == nullptr) {
        cout << "  Error: no order with ID " << id << ".\n";
        return false;
    }
    printOrderHeader();
    printOrderRow(*o);
    const Customer* c = customers.search(o->customerId);
    if (c != nullptr) cout << "  Customer: " << c->name << " (" << c->phone << ")\n";
    if (o->status == OrderStatus::WAITING)
        cout << "  Location: sewing queue, position " << positionInQueue(id) << " of "
             << sewingQueue.size() << "\n";
    else if (o->status == OrderStatus::COMPLETED)
        cout << "  Location: finished rack, waiting for pickup\n";
    else
        cout << "  Location: delivered to customer\n";
    return true;
}

Order* Workshop::findOrder(int id) { return orders.search(id); }

void Workshop::listOrdersInorder() const {
    if (orders.isEmpty()) {
        cout << "  No orders in the system.\n";
        return;
    }
    cout << "  All orders, inorder traversal (ascending ID) - " << orders.size() << " total:\n";
    printOrderHeader();
    orders.inorder([](const Order& o) { printOrderRow(o); });
}

void Workshop::showTreePreorder() const {
    if (orders.isEmpty()) {
        cout << "  The order tree is empty.\n";
        return;
    }
    cout << "  Preorder traversal (indent = depth). Nodes: " << orders.size()
         << ", height: " << orders.height() << "\n";
    orders.preorder([](const Order& o, int depth) {
        cout << "  " << std::string(depth * 4, ' ') << "[" << o.id << "] "
             << o.garment << " (" << statusToString(o.status) << ")\n";
    });
}

bool Workshop::updateOrder(int id, const std::string* garment, const std::string* alteration,
                           const double* price, const int* customerId) {
    Order* o = orders.search(id);
    if (o == nullptr) {
        cout << "  Error: no order with ID " << id << ".\n";
        return false;
    }
    if (o->status == OrderStatus::PICKED_UP) {
        cout << "  Error: order #" << id << " was already picked up; archived orders cannot be edited.\n";
        return false;
    }
    if (customerId != nullptr && customers.search(*customerId) == nullptr) {
        cout << "  Error: customer ID " << *customerId << " does not exist. No changes made.\n";
        return false;
    }
    if (garment != nullptr) o->garment = *garment;
    if (alteration != nullptr) o->alteration = *alteration;
    if (price != nullptr) o->price = *price;
    if (customerId != nullptr) o->customerId = *customerId;
    cout << "  Order #" << id << " updated:\n";
    printOrderHeader();
    printOrderRow(*o);
    return true;
}

// Scenario: an order was registered under the wrong ID. The key changes, so the
// BST node must be removed and re-inserted, and the copy of the ID held in the
// queue or stack is rewritten IN PLACE so the order keeps its position.
bool Workshop::changeOrderId(int oldId, int newId) {
    Order* o = orders.search(oldId);
    if (o == nullptr) {
        cout << "  Error: no order with ID " << oldId << ".\n";
        return false;
    }
    if (oldId == newId) {
        cout << "  Error: the new ID is the same as the old ID. Nothing changed.\n";
        return false;
    }
    if (orders.search(newId) != nullptr) {
        cout << "  Error: order ID " << newId << " is already in use. Duplicate IDs are rejected.\n";
        return false;
    }
    Order copy = *o;
    copy.id = newId;
    orders.remove(oldId);
    orders.insert(copy);

    std::string where = "BST only (order is picked up)";
    if (copy.status == OrderStatus::WAITING) {
        replaceInQueue(oldId, newId);
        where = "BST + sewing queue (position " + std::to_string(positionInQueue(newId)) + " kept)";
    } else if (copy.status == OrderStatus::COMPLETED) {
        replaceInStack(oldId, newId);
        where = "BST + finished stack (position kept)";
    }
    cout << "  Order #" << oldId << " is now #" << newId << ". Updated: " << where << ".\n";
    return true;
}

bool Workshop::deleteOrder(int id) {
    const Order* o = orders.search(id);
    if (o == nullptr) {
        cout << "  Error: no order with ID " << id << ".\n";
        return false;
    }
    std::string where = "BST";
    if (o->status == OrderStatus::WAITING) {
        removeFromQueue(id);
        where += " + sewing queue";
    } else if (o->status == OrderStatus::COMPLETED) {
        removeFromStack(id);
        where += " + finished stack";
    }
    orders.remove(id);
    cout << "  Order #" << id << " deleted from " << where << ". Orders remaining: "
         << orders.size() << "\n";
    return true;
}

// =============================== workflow =================================

void Workshop::showQueue() const {
    if (sewingQueue.isEmpty()) {
        cout << "  The sewing queue is empty (0/" << QUEUE_CAPACITY << ").\n";
        return;
    }
    cout << "  Sewing queue (" << sewingQueue.size() << "/" << QUEUE_CAPACITY
         << "), front first:\n";
    const OrderBST& tree = orders;
    sewingQueue.forEachFrontToRear([&tree](int pos, int id) {
        const Order* o = tree.search(id);
        cout << "   " << pos << ". #" << id;
        if (o != nullptr) cout << "  " << o->garment << " - " << o->alteration;
        cout << "\n";
    });
}

bool Workshop::sewNext() {
    if (sewingQueue.isEmpty()) {
        cout << "  Nothing to sew: the sewing queue is empty.\n";
        return false;
    }
    int id = sewingQueue.dequeue();
    Order* o = orders.search(id);
    if (o == nullptr) {   // cannot happen while the invariants hold
        cout << "  Internal error: queued order #" << id << " not found in BST.\n";
        return false;
    }
    o->status = OrderStatus::COMPLETED;
    finished.push(id);
    cout << "  Sewed order #" << id << " (" << o->garment << ": " << o->alteration << ").\n"
         << "  Status -> COMPLETED. Pushed onto finished stack (size " << finished.size()
         << "). Queue now holds " << sewingQueue.size() << ".\n";
    return true;
}

void Workshop::showFinished() const {
    if (finished.isEmpty()) {
        cout << "  No finished alterations are waiting for pickup.\n";
        return;
    }
    cout << "  Recently finished alterations (" << finished.size() << "), most recent first:\n";
    const OrderBST& tree = orders;
    finished.forEachTopDown([&tree](int pos, int id) {
        const Order* o = tree.search(id);
        cout << "   " << pos << ". #" << id;
        if (o != nullptr) cout << "  " << o->garment << " - " << o->alteration;
        if (pos == 1) cout << "   <- top";
        cout << "\n";
    });
}

// Scenario: quality control rejects the most recently finished garment (LIFO).
bool Workshop::reworkLatest() {
    if (finished.isEmpty()) {
        cout << "  Nothing to rework: the finished stack is empty.\n";
        return false;
    }
    if (sewingQueue.isFull()) {
        cout << "  Error: sewing queue is full; cannot send order #" << finished.peek()
             << " back for rework right now.\n";
        return false;
    }
    int id = finished.pop();
    Order* o = orders.search(id);
    o->status = OrderStatus::WAITING;
    sewingQueue.enqueue(id);
    cout << "  Order #" << id << " failed quality check: popped from finished stack and "
         << "re-queued for sewing (position " << sewingQueue.size() << ").\n";
    return true;
}

bool Workshop::pickUp(int id) {
    Order* o = orders.search(id);
    if (o == nullptr) {
        cout << "  Error: no order with ID " << id << ".\n";
        return false;
    }
    if (o->status == OrderStatus::WAITING) {
        cout << "  Error: order #" << id << " is not sewn yet (queue position "
             << positionInQueue(id) << ").\n";
        return false;
    }
    if (o->status == OrderStatus::PICKED_UP) {
        cout << "  Error: order #" << id << " was already picked up.\n";
        return false;
    }
    removeFromStack(id);
    o->status = OrderStatus::PICKED_UP;
    cout << "  Order #" << id << " handed to the customer. Amount due: " << std::fixed
         << std::setprecision(2) << o->price << " EGP. Status -> PICKED_UP.\n";
    return true;
}

void Workshop::showSummary() const {
    int waiting = 0, completed = 0, picked = 0;
    double revenue = 0.0;
    orders.inorder([&](const Order& o) {
        if (o.status == OrderStatus::WAITING) ++waiting;
        else if (o.status == OrderStatus::COMPLETED) ++completed;
        else { ++picked; revenue += o.price; }
    });
    cout << "  ===== Workshop summary =====\n"
         << "  Customers (linked list) : " << customers.size() << "\n"
         << "  Orders (BST)            : " << orders.size() << "   tree height " << orders.height() << "\n"
         << "  Sewing queue            : " << sewingQueue.size() << "/" << QUEUE_CAPACITY
         << (sewingQueue.isFull() ? "  (FULL)" : "") << "\n"
         << "  Finished stack          : " << finished.size() << "\n"
         << "  Status counts           : WAITING " << waiting << ", COMPLETED " << completed
         << ", PICKED_UP " << picked << "\n"
         << "  Revenue collected       : " << std::fixed << std::setprecision(2) << revenue << " EGP\n";
    checkConsistency(true);
}

bool Workshop::checkConsistency(bool verbose) const {
    bool ok = true;
    int waiting = 0, completed = 0;
    const OrderBST& tree = orders;
    const CustomerList& list = customers;
    orders.inorder([&](const Order& o) {
        if (o.status == OrderStatus::WAITING) ++waiting;
        if (o.status == OrderStatus::COMPLETED) ++completed;
        if (list.search(o.customerId) == nullptr) ok = false;   // I5
    });
    if (waiting != sewingQueue.size() || completed != finished.size()) ok = false;  // I2, I3
    sewingQueue.forEachFrontToRear([&](int, int id) {
        const Order* o = tree.search(id);
        if (o == nullptr || o->status != OrderStatus::WAITING) ok = false;
    });
    finished.forEachTopDown([&](int, int id) {
        const Order* o = tree.search(id);
        if (o == nullptr || o->status != OrderStatus::COMPLETED) ok = false;
    });
    if (verbose) cout << "  Consistency check       : " << (ok ? "OK" : "FAILED") << "\n";
    return ok;
}

void Workshop::loadSampleData() {
    cout << "  Loading sample data (duplicates are rejected, so loading twice is safe)...\n";
    addCustomer({101, "Mona Hassan",  "01001234567", {92, 74, 98, 78, 58}});
    addCustomer({102, "Ahmed Fathy",  "01112223334", {104, 90, 100, 82, 63}});
    addCustomer({103, "Sara Adel",    "01223344556", {86, 68, 94, 76, 56}});
    addCustomer({104, "Karim Nabil",  "01556677889", {110, 96, 104, 84, 65}});
    createOrder({5005, 102, "Suit jacket",   "Shorten sleeves 2 cm", 350.0, OrderStatus::WAITING});
    createOrder({3003, 101, "Evening dress", "Take in waist 3 cm",   420.0, OrderStatus::WAITING});
    createOrder({7007, 103, "Jeans",         "Hem to 76 cm inseam",  120.0, OrderStatus::WAITING});
    createOrder({2002, 104, "Shirt",         "Slim fit sides",       180.0, OrderStatus::WAITING});
    createOrder({6006, 101, "Coat",          "Replace lining",       650.0, OrderStatus::WAITING});
}

// ===================== queue / stack consistency helpers ==================
// These use only enqueue/dequeue/size and push/pop/isEmpty, so the ADTs stay pure.

bool Workshop::removeFromQueue(int id) {
    bool found = false;
    int n = sewingQueue.size();
    for (int i = 0; i < n; ++i) {
        int x = sewingQueue.dequeue();
        if (x == id && !found) found = true;      // drop it
        else sewingQueue.enqueue(x);              // rotate the rest, order preserved
    }
    return found;
}

bool Workshop::replaceInQueue(int oldId, int newId) {
    bool found = false;
    int n = sewingQueue.size();
    for (int i = 0; i < n; ++i) {
        int x = sewingQueue.dequeue();
        if (x == oldId) { x = newId; found = true; }
        sewingQueue.enqueue(x);
    }
    return found;
}

int Workshop::positionInQueue(int id) const {
    int position = -1;
    sewingQueue.forEachFrontToRear([&](int pos, int x) { if (x == id && position == -1) position = pos; });
    return position;
}

bool Workshop::removeFromStack(int id) {
    Stack<int> temp;
    bool found = false;
    while (!finished.isEmpty()) {
        int x = finished.pop();
        if (x == id) { found = true; break; }
        temp.push(x);
    }
    while (!temp.isEmpty()) finished.push(temp.pop());   // restore original order
    return found;
}

bool Workshop::replaceInStack(int oldId, int newId) {
    Stack<int> temp;
    bool found = false;
    while (!finished.isEmpty()) {
        int x = finished.pop();
        if (x == oldId) { finished.push(newId); found = true; break; }
        temp.push(x);
    }
    while (!temp.isEmpty()) finished.push(temp.pop());
    return found;
}
