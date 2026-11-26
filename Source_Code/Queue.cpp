// Queue.cpp
// CS212 Project 94 - Tailoring Order Workshop (simplified course-style version)

#include "Queue.h"

void queueInit(Queue& q) {
    q.front = 0;
    q.count = 0;
    q.capacity = 8;
}

bool queueIsEmpty(Queue& q) { return q.count == 0; }

bool queueIsFull(Queue& q) { return q.count == q.capacity; }

int queueSize(Queue& q) { return q.count; }

bool enqueue(Queue& q, int value) {
    if (queueIsFull(q)) return false;
    int rear = (q.front + q.count) % q.capacity;   // circular: wraps around
    q.items[rear] = value;
    q.count++;
    return true;
}

bool dequeue(Queue& q, int& value) {
    if (queueIsEmpty(q)) return false;
    value = q.items[q.front];
    q.front = (q.front + 1) % q.capacity;
    q.count--;
    return true;
}

bool queueFront(Queue& q, int& value) {
    if (queueIsEmpty(q)) return false;
    value = q.items[q.front];
    return true;
}

int queueAt(Queue& q, int position) {
    return q.items[(q.front + position) % q.capacity];
}
