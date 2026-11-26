// Stack.h
// CS212 Project 94 - Tailoring Order Workshop (simplified course-style version)
// Stack of order IDs written by hand with linked nodes (LIFO, no STL).
// Used for "recently finished alterations": the most recently completed
// order is on top, so quality-control rework always targets the newest one.
#ifndef STACK_H
#define STACK_H

#include <cstddef>

struct StackNode {
    int data;
    StackNode* next;
};

struct Stack {
    StackNode* top;   // NULL when the stack is empty
    int count;
};

void stackInit(Stack& s);
void stackPush(Stack& s, int value);                // O(1)
bool stackPop(Stack& s, int& value);                // O(1), false if empty ("Stack Underflow")
bool stackPeek(Stack& s, int& value);         // O(1), false if empty
bool stackIsEmpty(Stack& s);
int stackSize(Stack& s);
void stackClear(Stack& s);

#endif // STACK_H
