// Customer.h
// CS212 Project 94 - Tailoring Order Workshop (simplified course-style version)
// Customer record with body measurements (stored in the linked list).
#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <string>

using namespace std;

struct Measurements {
    double chest;    // all values in centimetres
    double waist;
    double hips;
    double inseam;
    double sleeve;
};

struct Customer {
    int id;          // unique key (1..99999)
    string name;
    string phone;
    Measurements m;
};

#endif // CUSTOMER_H
