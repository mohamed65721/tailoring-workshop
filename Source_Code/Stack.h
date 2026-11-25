// Stack.h
// CS212 Project 94 - Tailoring Order Workshop
// Manually implemented LIFO stack (singly linked nodes, no STL containers).
// Used for "recently finished alterations": the most recently completed
// order is on top, so quality-control rework always targets the newest one.
#ifndef STACK_H
#define STACK_H

#include <stdexcept>

template <typename T>
class Stack {
private:
    struct Node {
        T data;
        Node* next;
        Node(const T& d, Node* n) : data(d), next(n) {}
    };
    Node* topNode;
    int count;

public:
    Stack() : topNode(nullptr), count(0) {}
    ~Stack() { clear(); }

    // Copying is disabled: the workshop owns exactly one stack.
    Stack(const Stack&) = delete;
    Stack& operator=(const Stack&) = delete;

    // O(1)
    void push(const T& value) {
        topNode = new Node(value, topNode);
        ++count;
    }

    // O(1). Throws std::underflow_error if the stack is empty.
    T pop() {
        if (isEmpty()) throw std::underflow_error("pop() on empty stack");
        Node* old = topNode;
        T value = old->data;
        topNode = old->next;
        delete old;
        --count;
        return value;
    }

    // O(1). Throws std::underflow_error if the stack is empty.
    T peek() const {
        if (isEmpty()) throw std::underflow_error("peek() on empty stack");
        return topNode->data;
    }

    bool isEmpty() const { return topNode == nullptr; }
    int size() const { return count; }

    void clear() {
        while (topNode != nullptr) {
            Node* old = topNode;
            topNode = topNode->next;
            delete old;
        }
        count = 0;
    }

    // Read-only walk from top to bottom (for display). O(n)
    template <typename Visitor>
    void forEachTopDown(Visitor visit) const {
        int position = 1;
        for (Node* cur = topNode; cur != nullptr; cur = cur->next) {
            visit(position++, cur->data);
        }
    }
};

#endif // STACK_H
