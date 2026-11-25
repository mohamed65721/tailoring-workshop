// main.cpp
// CS212 Project 94 - Tailoring Order Workshop
// Console menu. Reads and validates input, then calls the Workshop coordinator.
#include "InputUtils.h"
#include "Workshop.h"

#include <iostream>
#include <stdexcept>
#include <string>

using std::cout;

static void printMenu() {
    cout << "\n================ TAILORING ORDER WORKSHOP ================\n"
         << " Customers (linked list)        Orders (BST by order ID)\n"
         << "  1. Add customer                 7. Create order\n"
         << "  2. List customers               8. Find order by ID\n"
         << "  3. Find customer by ID          9. List orders (inorder)\n"
         << "  4. Update customer             10. Show tree (preorder)\n"
         << "  5. Delete customer             11. Update order details\n"
         << "  6. Sort customers (merge sort) 12. Correct an order ID\n"
         << "                                 13. Delete / cancel order\n"
         << " Workflow (queue -> stack)\n"
         << " 14. View sewing queue           17. Rework latest finished\n"
         << " 15. Sew next garment            18. Customer pickup\n"
         << " 16. View finished stack         19. Workshop summary\n"
         << " 20. Load sample data             0. Exit\n"
         << "==========================================================\n";
}

static Measurements readMeasurements() {
    Measurements m;
    m.chest  = readDouble("  Chest (cm, 30-200): ", 30, 200);
    m.waist  = readDouble("  Waist (cm, 30-200): ", 30, 200);
    m.hips   = readDouble("  Hips (cm, 30-200): ", 30, 200);
    m.inseam = readDouble("  Inseam (cm, 30-130): ", 30, 130);
    m.sleeve = readDouble("  Sleeve (cm, 30-100): ", 30, 100);
    return m;
}

static int readId(const std::string& what) {
    return readInt("  " + what + " (" + std::to_string(Workshop::MIN_ID) + "-" +
                       std::to_string(Workshop::MAX_ID) + "): ",
                   Workshop::MIN_ID, Workshop::MAX_ID);
}

static void addCustomer(Workshop& w) {
    Customer c;
    c.id = readId("Customer ID");
    if (w.findCustomer(c.id) != nullptr) {   // fail fast before asking for everything
        cout << "  Error: customer ID " << c.id << " already exists. Duplicate IDs are not allowed.\n";
        return;
    }
    c.name = readNonEmptyLine("  Name: ", 40);
    c.phone = readNonEmptyLine("  Phone: ", 20);
    c.m = readMeasurements();
    w.addCustomer(c);
}

static void updateCustomer(Workshop& w) {
    int id = readId("Customer ID to update");
    Customer* c = w.findCustomer(id);
    if (c == nullptr) {
        cout << "  Error: no customer with ID " << id << ".\n";
        return;
    }
    cout << "  Press Enter to keep the current value.\n";
    std::string s;
    if (readOptionalLine("  Name [" + c->name + "]: ", s, 40)) c->name = s;
    if (readOptionalLine("  Phone [" + c->phone + "]: ", s, 20)) c->phone = s;
    if (readYesNo("  Re-take measurements? (y/n): ")) c->m = readMeasurements();
    cout << "  Customer #" << id << " updated.\n";
    w.showCustomer(id);
}

static void createOrder(Workshop& w) {
    Order o;
    o.id = readId("Order ID");
    if (w.findOrder(o.id) != nullptr) {
        cout << "  Error: order ID " << o.id << " already exists. Duplicate IDs are rejected.\n";
        return;
    }
    o.customerId = readId("Customer ID");
    if (w.findCustomer(o.customerId) == nullptr) {
        cout << "  Error: customer ID " << o.customerId << " does not exist. Add the customer first.\n";
        return;
    }
    o.garment = readNonEmptyLine("  Garment: ", 40);
    o.alteration = readNonEmptyLine("  Alteration needed: ", 60);
    o.price = readDouble("  Price (EGP, 0-100000): ", 0, 100000);
    w.createOrder(o);
}

static void updateOrder(Workshop& w) {
    int id = readId("Order ID to update");
    Order* o = w.findOrder(id);
    if (o == nullptr) {
        cout << "  Error: no order with ID " << id << ".\n";
        return;
    }
    cout << "  Press Enter to keep the current value.\n";
    std::string garment, alteration, priceText, custText;
    bool g = readOptionalLine("  Garment [" + o->garment + "]: ", garment, 40);
    bool a = readOptionalLine("  Alteration [" + o->alteration + "]: ", alteration, 60);
    double price = o->price;
    bool p = readYesNo("  Change price? (y/n): ");
    if (p) price = readDouble("  New price (EGP, 0-100000): ", 0, 100000);
    int cust = o->customerId;
    bool c = readYesNo("  Re-assign to another customer? (y/n): ");
    if (c) cust = readId("New customer ID");
    w.updateOrder(id, g ? &garment : nullptr, a ? &alteration : nullptr,
                  p ? &price : nullptr, c ? &cust : nullptr);
}

static void handleChoice(Workshop& w, int choice) {
    switch (choice) {
        case 1: addCustomer(w); break;
        case 2: w.listCustomers(); break;
        case 3: w.showCustomer(readId("Customer ID")); break;
        case 4: updateCustomer(w); break;
        case 5: w.deleteCustomer(readId("Customer ID to delete")); break;
        case 6: {
            int k = readInt("  Sort by 1) name  2) ID: ", 1, 2);
            w.sortCustomers(k == 1 ? CustomerSortKey::BY_NAME : CustomerSortKey::BY_ID);
            break;
        }
        case 7: createOrder(w); break;
        case 8: w.showOrder(readId("Order ID")); break;
        case 9: w.listOrdersInorder(); break;
        case 10: w.showTreePreorder(); break;
        case 11: updateOrder(w); break;
        case 12: {
            int oldId = readId("Current (wrong) order ID");
            if (w.findOrder(oldId) == nullptr) {
                cout << "  Error: no order with ID " << oldId << ".\n";
                break;
            }
            int newId = readId("Correct order ID");
            w.changeOrderId(oldId, newId);
            break;
        }
        case 13: {
            int id = readId("Order ID to delete");
            if (w.findOrder(id) == nullptr) {
                cout << "  Error: no order with ID " << id << ".\n";
                break;
            }
            if (readYesNo("  Really delete order #" + std::to_string(id) + "? (y/n): "))
                w.deleteOrder(id);
            else
                cout << "  Delete cancelled.\n";
            break;
        }
        case 14: w.showQueue(); break;
        case 15: w.sewNext(); break;
        case 16: w.showFinished(); break;
        case 17: w.reworkLatest(); break;
        case 18: w.pickUp(readId("Order ID being picked up")); break;
        case 19: w.showSummary(); break;
        case 20: w.loadSampleData(); break;
        default: break;
    }
}

int main() {
    Workshop workshop;
    cout << "Welcome to the Tailoring Order Workshop (CS212 Project 94).\n"
         << "Tip: choose 20 to load sample data.\n";
    try {
        while (true) {
            printMenu();
            int choice = readInt("Choose an option (0-20): ", 0, 20);
            if (choice == 0) break;
            try {
                handleChoice(workshop, choice);
            } catch (const InputEnded&) {
                throw;                                       // handled below
            } catch (const std::exception& e) {              // safety net, should not trigger
                cout << "  Unexpected error: " << e.what() << "\n";
            }
        }
    } catch (const InputEnded&) {
        cout << "\n(Input ended.)\n";
    }
    cout << "Goodbye.\n";
    return 0;
}
