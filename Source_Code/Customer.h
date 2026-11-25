// Customer.h
// CS212 Project 94 - Tailoring Order Workshop
// Customer record with body measurements (stored in the linked list).
#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <string>

struct Measurements {
    double chest = 0.0;   // all values in centimetres
    double waist = 0.0;
    double hips = 0.0;
    double inseam = 0.0;
    double sleeve = 0.0;
};

struct Customer {
    int id = 0;            // unique key (1..99999)
    std::string name;
    std::string phone;
    Measurements m;
};

#endif // CUSTOMER_H
