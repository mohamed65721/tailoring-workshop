// Order.cpp
// CS212 Project 94 - Tailoring Order Workshop (simplified course-style version)

#include <string>
#include "Order.h"

using namespace std;

string statusToString(int s) {
    if (s == 0 /*WAITING*/) return "WAITING";
    if (s == 1 /*COMPLETED*/) return "COMPLETED";
    if (s == 2 /*PICKED_UP*/) return "PICKED_UP";
    return "UNKNOWN";
}
