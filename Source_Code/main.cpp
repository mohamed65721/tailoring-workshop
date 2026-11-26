// main.cpp
// CS212 Project 94 - Tailoring Order Workshop (simplified course-style version)
// Console menu. Reads and validates input, then calls the Workshop coordinator.

#include <iostream>
#include <string>
#include "Workshop.h"
#include "InputUtils.h"

using namespace std;

void printMenu() {
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

Measurements readMeasurements() {
    Measurements m;
    m.chest  = readDouble("  Chest (cm, 30-200): ", 30, 200);
    m.waist  = readDouble("  Waist (cm, 30-200): ", 30, 200);
    m.hips   = readDouble("  Hips (cm, 30-200): ", 30, 200);
    m.inseam = readDouble("  Inseam (cm, 30-130): ", 30, 130);
    m.sleeve = readDouble("  Sleeve (cm, 30-100): ", 30, 100);   // if input ended, the caller checks inputHasEnded()
    return m;
}

int readId(string what) {
    return readInt("  " + what + " (1-99999): ",
                   1, 99999);
}

void menuAddCustomer() {
    Customer c;
    c.id = readId("Customer ID");
    if (inputHasEnded()) return;
    if (findCustomer(c.id) != NULL) {   // fail fast before asking for everything
        cout << "  Error: customer ID " << c.id << " already exists. Duplicate IDs are not allowed.\n";
        return;
    }
    c.name = readNonEmptyLine("  Name: ", 40);
    if (inputHasEnded()) return;
    c.phone = readNonEmptyLine("  Phone: ", 20);
    if (inputHasEnded()) return;
    c.m = readMeasurements();
    if (inputHasEnded()) return;
    addCustomer(c);
}

void menuUpdateCustomer() {
    int id = readId("Customer ID to update");
    if (inputHasEnded()) return;
    Customer* c = findCustomer(id);
    if (c == NULL) {
        cout << "  Error: no customer with ID " << id << ".\n";
        return;
    }
    cout << "  Press Enter to keep the current value.\n";
    string s;
    if (readOptionalLine("  Name [" + c->name + "]: ", s, 40)) c->name = s;
    if (inputHasEnded()) return;
    if (readOptionalLine("  Phone [" + c->phone + "]: ", s, 20)) c->phone = s;
    if (inputHasEnded()) return;
    bool retake = readYesNo("  Re-take measurements? (y/n): ");
    if (inputHasEnded()) return;
    if (retake) {
        Measurements m = readMeasurements();
        if (inputHasEnded()) return;
        c->m = m;
    }
    cout << "  Customer #" << id << " updated.\n";
    showCustomer(id);
}

void menuCreateOrder() {
    Order o;
    o.id = readId("Order ID");
    if (inputHasEnded()) return;
    if (findOrder(o.id) != NULL) {
        cout << "  Error: order ID " << o.id << " already exists. Duplicate IDs are rejected.\n";
        return;
    }
    o.customerId = readId("Customer ID");
    if (inputHasEnded()) return;
    if (findCustomer(o.customerId) == NULL) {
        cout << "  Error: customer ID " << o.customerId << " does not exist. Add the customer first.\n";
        return;
    }
    o.garment = readNonEmptyLine("  Garment: ", 40);
    if (inputHasEnded()) return;
    o.alteration = readNonEmptyLine("  Alteration needed: ", 60);
    if (inputHasEnded()) return;
    o.price = readDouble("  Price (EGP, 0-100000): ", 0, 100000);
    if (inputHasEnded()) return;
    o.status = 0 /*WAITING*/;
    createOrder(o);
}

void menuUpdateOrder() {
    int id = readId("Order ID to update");
    if (inputHasEnded()) return;
    Order* o = findOrder(id);
    if (o == NULL) {
        cout << "  Error: no order with ID " << id << ".\n";
        return;
    }
    cout << "  Press Enter to keep the current value.\n";
    string garment, alteration, priceText, custText;
    bool g = readOptionalLine("  Garment [" + o->garment + "]: ", garment, 40);
    if (inputHasEnded()) return;
    bool a = readOptionalLine("  Alteration [" + o->alteration + "]: ", alteration, 60);
    if (inputHasEnded()) return;
    double price = o->price;
    bool p = readYesNo("  Change price? (y/n): ");
    if (inputHasEnded()) return;
    if (p) price = readDouble("  New price (EGP, 0-100000): ", 0, 100000);
    if (inputHasEnded()) return;
    int cust = o->customerId;
    bool c = readYesNo("  Re-assign to another customer? (y/n): ");
    if (inputHasEnded()) return;
    if (c) cust = readId("New customer ID");
    if (inputHasEnded()) return;
    string* gp = NULL;
    string* ap = NULL;
    double* pp = NULL;
    int* cp = NULL;
    if (g) gp = &garment;
    if (a) ap = &alteration;
    if (p) pp = &price;
    if (c) cp = &cust;
    updateOrder(id, gp, ap, pp, cp);
}

void handleChoice(int choice) {
    switch (choice) {
        case 1: menuAddCustomer(); break;
        case 2: listCustomers(); break;
        case 3: {
            int id = readId("Customer ID");
            if (inputHasEnded()) break;
            showCustomer(id);
            break;
        }
        case 4: menuUpdateCustomer(); break;
        case 5: {
            int id = readId("Customer ID to delete");
            if (inputHasEnded()) break;
            deleteCustomer(id);
            break;
        }
        case 6: {
            int k = readInt("  Sort by 1) name  2) ID: ", 1, 2);
            if (inputHasEnded()) break;
            if (k == 1) sortCustomers(1 /*by name*/);
            else sortCustomers(2 /*by ID*/);
            break;
        }
        case 7: menuCreateOrder(); break;
        case 8: {
            int id = readId("Order ID");
            if (inputHasEnded()) break;
            showOrder(id);
            break;
        }
        case 9: listOrdersInorder(); break;
        case 10: showTreePreorder(); break;
        case 11: menuUpdateOrder(); break;
        case 12: {
            int oldId = readId("Current (wrong) order ID");
            if (inputHasEnded()) break;
            if (findOrder(oldId) == NULL) {
                cout << "  Error: no order with ID " << oldId << ".\n";
                break;
            }
            int newId = readId("Correct order ID");
            if (inputHasEnded()) break;
            changeOrderId(oldId, newId);
            break;
        }
        case 13: {
            int id = readId("Order ID to delete");
            if (inputHasEnded()) break;
            if (findOrder(id) == NULL) {
                cout << "  Error: no order with ID " << id << ".\n";
                break;
            }
            bool sure = readYesNo("  Really delete order #" + intToText(id) + "? (y/n): ");
            if (inputHasEnded()) break;
            if (sure)
                deleteOrder(id);
            else
                cout << "  Delete cancelled.\n";
            break;
        }
        case 14: showQueue(); break;
        case 15: sewNext(); break;
        case 16: showFinished(); break;
        case 17: reworkLatest(); break;
        case 18: {
            int id = readId("Order ID being picked up");
            if (inputHasEnded()) break;
            pickUp(id);
            break;
        }
        case 19: showSummary(); break;
        case 20: loadSampleData(); break;
        default: break;
    }
}

int main() {
    initWorkshop();
    cout << "Welcome to the Tailoring Order Workshop (CS212 Project 94).\n"
         << "Tip: choose 20 to load sample data.\n";
    while (true) {
        printMenu();
        int choice = readInt("Choose an option (0-20): ", 0, 20);
        if (inputHasEnded() || choice == 0) break;
        handleChoice(choice);
        if (inputHasEnded()) break;
    }
    cout << "Goodbye.\n";
    closeWorkshop();
    return 0;
}
