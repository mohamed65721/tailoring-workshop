// Queue.h
// CS212 Project 94 - Tailoring Order Workshop (simplified course-style version)
// Queue of order IDs (FIFO) on a fixed-size circular array, no STL.
// Used for garments awaiting sewing. The fixed capacity models the limited
// rack space in the workshop: when the rack is full, new orders are refused.
#ifndef QUEUE_H
#define QUEUE_H

struct Queue {
    int items[8];  // the sewing rack holds 8 garments
    int front;     // index of the current front element
    int count;     // number of stored elements
    int capacity;  // always 8 (set by queueInit)
};

void queueInit(Queue& q);
bool enqueue(Queue& q, int value);          // O(1), false if the queue is full
bool dequeue(Queue& q, int& value);         // O(1), false if the queue is empty
bool queueFront(Queue& q, int& value);
bool queueIsEmpty(Queue& q);
bool queueIsFull(Queue& q);
int queueSize(Queue& q);
int queueAt(Queue& q, int position);  // position 0 = front (for display)

#endif // QUEUE_H
