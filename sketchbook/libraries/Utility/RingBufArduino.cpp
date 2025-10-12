#if 0 // Test Version 240511-1
#include "RingBufArduino.h"

// Replace EXTMEM with your platform-specific keyword for external PSRAM allocation
#define EXTMEM PSRAM_XMEM

// Constructor: Initializes the ring buffer with a specified length
template <typename T>
RingBufArduino<T>::RingBufArduino(size_t len)
    : bufferLen(len), head(0), tail(0), count(0) {
    // Allocate memory for the buffer in EXTMEM
    buffer = (T*)EXTMEM_ALLOC(len * sizeof(T));
}

// Destructor: Deallocates memory used by the ring buffer
template <typename T>
RingBufArduino<T>::~RingBufArduino() {
    // Free the memory allocated for the buffer
    EXTMEM_FREE(buffer);
}

// Add an element to the ring buffer
template <typename T>
bool RingBufArduino<T>::push(const T& val) {
    if (count == bufferLen) {
        // Buffer is full, cannot add element
        return false;
    }

    // Add the element at the tail position
    buffer[tail] = val;

    // Update the tail position
    tail = (tail + 1) % bufferLen;

    // Increment the count of elements
    count++;

    return true;
}

// Remove an element from the ring buffer
template <typename T>
T RingBufArduino<T>::pop() {
    if (count == 0) {
        // Buffer is empty, return a default value
        return T();
    }

    // Get the element at the head position
    T val = buffer[head];

    // Update the head position
    head = (head + 1) % bufferLen;

    // Decrement the count of elements
    count--;

    return val;
}

// Get the number of elements currently in the ring buffer
template <typename T>
size_t RingBufArduino<T>::size() const {
    return count;
}

// Clear the contents of the ring buffer
template <typename T>
void RingBufArduino<T>::clear() {
    head = 0;
    tail = 0;
    count = 0;
}
#endif // Test Version 240511-1



#if 1 // Working Version 240509-1
/**
 * @file RingBufArduino.cpp
 * @brief This file contains the implementation of the RingBufArduino class,
 *        which is a generic ring buffer data structure compatible with Arduino 2.0.
 */

#include "RingBufArduino.h"

/**
 * @brief Constructs a RingBufArduino object with a specified size.
 *
 * @param size The maximum number of elements the ring buffer can hold.
 */
template <typename T>
RingBufArduino<T>::RingBufArduino(size_t size) : head_(0), tail_(0), count_(0), max_size_(size) {
    if (size == 0) {
        buf_ = nullptr;
        return;
    }

    buf_ = new T[size];
}

/**
 * @brief Destructor for the RingBufArduino class.
 */
template <typename T>
RingBufArduino<T>::~RingBufArduino() {
    delete[] buf_;
}

/**
 * @brief Adds an element to the back of the ring buffer.
 *
 * @param value The element to be added.
 * @return true if the element was successfully added, false otherwise.
 */
template <typename T>
bool RingBufArduino<T>::enqueue(const T& value) {
    if (count_ == max_size_) {
        // Buffer is full, cannot enqueue
        return false;
    }

    buf_[tail_] = value;
    tail_ = (tail_ + 1) % max_size_;
    ++count_;
    return true;
}

/**
 * @brief Removes an element from the front of the ring buffer.
 *
 * @param value A reference to store the removed element.
 * @return true if an element was successfully removed, false otherwise.
 */
template <typename T>
bool RingBufArduino<T>::dequeue(T& value) {
    if (count_ == 0) {
        // Buffer is empty, cannot dequeue
        return false;
    }

    value = buf_[head_];
    head_ = (head_ + 1) % max_size_;
    --count_;
    return true;
}

/**
 * @brief Returns the number of elements currently stored in the ring buffer.
 *
 * @return The number of elements in the ring buffer.
 */
template <typename T>
size_t RingBufArduino<T>::size() const {
    return count_;
}

/**
 * @brief Clears the contents of the ring buffer.
 */
template <typename T>
void RingBufArduino<T>::clear() {
    head_ = 0;
    tail_ = 0;
    count_ = 0;
}
#endif // Working Version 240509-1
