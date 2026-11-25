// Queue.h
// CS212 Project 94 - Tailoring Order Workshop
// Manually implemented FIFO queue on a fixed-size circular array.
// Used for garments awaiting sewing. The fixed capacity models the limited
// rack space in the workshop: when the rack is full, new orders are refused.
#ifndef QUEUE_H
#define QUEUE_H

#include <stdexcept>

template <typename T>
class Queue {
private:
    T* items;
    int capacity;
    int frontIndex;   // index of the current front element
    int count;        // number of stored elements

public:
    explicit Queue(int cap) : items(nullptr), capacity(cap), frontIndex(0), count(0) {
        if (cap <= 0) throw std::invalid_argument("Queue capacity must be positive");
        items = new T[cap];
    }
    ~Queue() { delete[] items; }

    Queue(const Queue&) = delete;
    Queue& operator=(const Queue&) = delete;

    // O(1). Returns false (and stores nothing) if the queue is full.
    bool enqueue(const T& value) {
        if (isFull()) return false;
        int rear = (frontIndex + count) % capacity;
        items[rear] = value;
        ++count;
        return true;
    }

    // O(1). Throws std::underflow_error if the queue is empty.
    T dequeue() {
        if (isEmpty()) throw std::underflow_error("dequeue() on empty queue");
        T value = items[frontIndex];
        frontIndex = (frontIndex + 1) % capacity;
        --count;
        return value;
    }

    // O(1). Throws std::underflow_error if the queue is empty.
    T front() const {
        if (isEmpty()) throw std::underflow_error("front() on empty queue");
        return items[frontIndex];
    }

    bool isEmpty() const { return count == 0; }
    bool isFull() const { return count == capacity; }
    int size() const { return count; }
    int getCapacity() const { return capacity; }

    // Read-only walk front-to-rear (for display). O(n)
    template <typename Visitor>
    void forEachFrontToRear(Visitor visit) const {
        for (int i = 0; i < count; ++i) {
            visit(i + 1, items[(frontIndex + i) % capacity]);
        }
    }
};

#endif // QUEUE_H
