// Stack.cpp
// CS212 Project 94 - Tailoring Order Workshop (simplified course-style version)

#include "Stack.h"

void stackInit(Stack& s) {
    s.top = NULL;
    s.count = 0;
}

void stackPush(Stack& s, int value) {
    StackNode* node = new StackNode;
    node->data = value;
    node->next = s.top;
    s.top = node;
    s.count++;
}

bool stackPop(Stack& s, int& value) {
    if (s.top == NULL) return false;      // underflow
    StackNode* old = s.top;
    value = old->data;
    s.top = old->next;
    delete old;
    s.count--;
    return true;
}

bool stackPeek(Stack& s, int& value) {
    if (s.top == NULL) return false;
    value = s.top->data;
    return true;
}

bool stackIsEmpty(Stack& s) { return s.top == NULL; }

int stackSize(Stack& s) { return s.count; }

void stackClear(Stack& s) {
    while (s.top != NULL) {
        StackNode* old = s.top;
        s.top = s.top->next;
        delete old;
    }
    s.count = 0;
}
